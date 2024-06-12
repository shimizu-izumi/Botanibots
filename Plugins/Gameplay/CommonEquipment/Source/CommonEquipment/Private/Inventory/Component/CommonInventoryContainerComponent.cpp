// Copyright © 2024 MajorT. All rights reserved.


#include "Inventory/Component/CommonInventoryContainerComponent.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "Inventory/Component/CommonEquipmentComponent.h"
#include "Inventory/Definition/CommonEquippableItemDefinition.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonInventoryContainerComponent)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_InventoryContainer_Message_SlotsChanged, "Inventory.Container.Message.SlotsChanged");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_InventoryContainer_Message_ActiveIndexChanged, "Inventory.Container.Message.ActiveIndexChanged");

/************************************************************************************************************************/
/* FCommonInventoryContainerEntry																						*/
/************************************************************************************************************************/

FGameplayTag FCommonInventoryContainerList::GetContainerTag(UCommonInventoryItemInstance* ItemInstance) const
{
	for (const auto& Entry : ContainerEntries)
	{
		if (Entry.Items.Contains(ItemInstance))
		{
			return Entry.Tag;
		}
	}

	return FGameplayTag();
}

UCommonInventoryItemInstance* FCommonInventoryContainerList::GetItem(const FGameplayTag& ContainerTag, const int32 ItemIndex) const
{
	for (const auto& Entry : ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			if (!Entry.Items.IsValidIndex(ItemIndex))
			{
				return nullptr;
			}

			return Entry.Items[ItemIndex].Get();
		}
	}

	return nullptr;
}

UCommonInventoryItemInstance* FCommonInventoryContainerList::GetActiveItem(const FGameplayTag& ContainerTag) const
{
	for (const auto& Entry : ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			if (!Entry.Items.IsValidIndex(Entry.ActiveItemIndex))
			{
				return nullptr;
			}

			return Entry.Items[Entry.ActiveItemIndex].Get();
		}
	}

	return nullptr;
}

int32 FCommonInventoryContainerList::GetActiveItemIndex(const FGameplayTag& ContainerTag) const
{
	for (const auto& Entry : ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			return Entry.ActiveItemIndex;
		}
	}

	return INDEX_NONE;
}

int32 FCommonInventoryContainerList::GetNextAvailableSlot(const FGameplayTag& ContainerTag) const
{
	for (const auto& Entry : ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			return Entry.Items.Num();
		}
	}

	return INDEX_NONE;
}

int32 FCommonInventoryContainerList::GetItemIndex(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance) const
{
	for (const auto& Entry : ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			return Entry.Items.IndexOfByKey(ItemInstance);
		}
	}

	return INDEX_NONE;
}

int32 FCommonInventoryContainerList::GetNumContainers() const
{
	return ContainerEntries.Num();
}

int32 FCommonInventoryContainerList::GetNumItems(const FGameplayTag& ContainerTag) const
{
	for (const auto& Entry : ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			return Entry.Items.Num();
		}
	}

	return INDEX_NONE;
}

int32 FCommonInventoryContainerList::AddEntry(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance)
{
	// Validate the inputs
	checkf(OwnerComponent != nullptr, TEXT("OwnerComponent is not set in: [%hs]"), __FUNCTION__);
	checkf(ItemInstance != nullptr, TEXT("ItemInstance is not set in: [%hs]"), __FUNCTION__);
	checkf(OwnerComponent->GetOwner()->HasAuthority(), TEXT("Owner Actor: [%s] is not authority in: [%hs]"), *OwnerComponent->GetOwner()->GetName(), __FUNCTION__);

	int32 Index = INDEX_NONE;

	for (FCommonInventoryContainerEntry& Entry : ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			if (Entry.MaxNumItems > 0 && Entry.Items.Num() < Entry.MaxNumItems)
			{
				Index = Entry.Items.Add(ItemInstance);
				MarkItemDirty(Entry);
			}
		}
	}

	return Index;
}

void FCommonInventoryContainerList::RemoveEntry(const FGameplayTag& ContainerTag, const int32 ItemIndex)
{
	RemoveEntry(ContainerTag, GetItem(ContainerTag, ItemIndex));
}

void FCommonInventoryContainerList::RemoveEntry(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance)
{
	for (auto EntryIterator = ContainerEntries.CreateIterator(); EntryIterator; ++EntryIterator)
	{
		FCommonInventoryContainerEntry& Entry = *EntryIterator;

		if (GetContainerTag(ItemInstance).MatchesTagExact(Entry.Tag))
		{
			EntryIterator.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

void FCommonInventoryContainerList::SetActiveSlot(const FGameplayTag& ContainerTag, const int32 ItemIndex)
{
	for (auto& Entry : ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			Entry.ActiveItemIndex = ItemIndex;
			MarkItemDirty(Entry);
		}
	}
}

void FCommonInventoryContainerList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FCommonInventoryContainerList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

void FCommonInventoryContainerList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

/************************************************************************************************************************/
/* UCommonInventoryContainerComponent																						*/
/************************************************************************************************************************/

UCommonInventoryContainerComponent::UCommonInventoryContainerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ContainerList(this)
{
}

int32 UCommonInventoryContainerComponent::AddItemToContainer(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance)
{
	int32 Index = ContainerList.AddEntry(ContainerTag, ItemInstance);
	OnRep_ContainerList();
	return Index;
}

void UCommonInventoryContainerComponent::RemoveItemFromContainer(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance)
{
	if (!ItemInstance)
	{
		return;
	}

	int32 ItemIndex = ContainerList.GetItemIndex(ContainerTag, ItemInstance);
	RemoveItemFromContainer(ContainerTag, ItemIndex);
}

void UCommonInventoryContainerComponent::RemoveItemFromContainer(const FGameplayTag& ContainerTag, int32 ItemIndex)
{
	if (ContainerList.GetActiveItemIndex(ContainerTag) == ItemIndex)
	{
		UnequipItemInContainer(ContainerTag, ItemIndex);
		int32 NextSlotIndex = ContainerList.GetNextAvailableSlot(ContainerTag);
		SetActiveItemInContainer(ContainerTag, NextSlotIndex);
	}

	if (ContainerList.GetItem(ContainerTag, ItemIndex))
	{
		ContainerList.RemoveEntry(ContainerTag, ItemIndex);
	}
}

void UCommonInventoryContainerComponent::SetActiveItemInContainer(const FGameplayTag& ContainerTag, int32 ItemIndex)
{
	if (ContainerList.GetActiveItemIndex(ContainerTag) == ItemIndex)
	{
		return;
	}

	UnequipItemInContainer(ContainerTag, ItemIndex);
	ContainerList.SetActiveSlot(ContainerTag, ItemIndex);

	// Equip the item in the container if there is one
	if (ContainerList.GetItem(ContainerTag, ItemIndex))
	{
		EquipItemInContainer(ContainerTag, ItemIndex);
	}
}

UCommonInventoryItemInstance* UCommonInventoryContainerComponent::GetActiveItemInContainer( const FGameplayTag& ContainerTag) const
{
	for (auto& Entry : ContainerList.ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			return Entry.Items.IsValidIndex(Entry.ActiveItemIndex) ? Entry.Items[Entry.ActiveItemIndex].Get() : nullptr;
		}
	}

	return nullptr;
}

UCommonInventoryItemInstance* UCommonInventoryContainerComponent::GetItemInContainer(const FGameplayTag& ContainerTag, int32 ItemIndex) const
{
	for (auto& Entry : ContainerList.ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			return Entry.Items.IsValidIndex(ItemIndex) ? Entry.Items[ItemIndex].Get() : nullptr;
		}
	}

	return nullptr;
}

TArray<UCommonInventoryItemInstance*> UCommonInventoryContainerComponent::GetAllItemsInContainer(const FGameplayTag& ContainerTag) const
{
	for (auto& Entry : ContainerList.ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			TArray<UCommonInventoryItemInstance*> Items;
			for (auto& Item : Entry.Items)
			{
				Items.Add(Item.Get());
			}

			return Items;
		}
	}

	return TArray<UCommonInventoryItemInstance*>();
}

int32 UCommonInventoryContainerComponent::GetActiveItemIndexInContainer(const FGameplayTag& ContainerTag) const
{
	for (auto& Entry : ContainerList.ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			return Entry.ActiveItemIndex;
		}
	}

	return INDEX_NONE;
}

int32 UCommonInventoryContainerComponent::GetNextAvailableIndexInContainer(const FGameplayTag& ContainerTag) const
{
	for (auto& Entry : ContainerList.ContainerEntries)
	{
		if (Entry.Tag.MatchesTagExact(ContainerTag))
		{
			for (int32 Index = 0; Index < Entry.MaxNumItems; ++Index)
			{
				if (!Entry.Items.IsValidIndex(Index))
				{
					return Index;
				}
				
				if (Entry.Items[Index] == nullptr)
				{
					return Index;
				}
			}
		}
	}

	return INDEX_NONE;
}

FGameplayTag UCommonInventoryContainerComponent::GetContainerTag(UCommonInventoryItemInstance* ItemInstance) const
{
	return ContainerList.GetContainerTag(ItemInstance);
}

void UCommonInventoryContainerComponent::InitInventory(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitInventory(InOwnerActor, InAvatarActor);
	
	for (const auto& InitialSlot : InitialContainerConfig->InitialContainer)
	{
		ContainerList.ContainerEntries.Add(FCommonInventoryContainerEntry(InitialSlot.ContainerTag, InitialSlot.MaxItems));
	}
}

void UCommonInventoryContainerComponent::EquipItemInContainer(const FGameplayTag& ContainerTag, int32 ItemIndex)
{
	check(ContainerList.GetItem(ContainerTag, ItemIndex));
	check(EquippedItems.Find(ContainerTag) == nullptr);

	UCommonInventoryItemInstance* ItemInstance = ContainerList.GetItem(ContainerTag, ItemIndex);
	if (!ItemInstance)
	{
		return;
	}

	if (ItemInstance->GetItemDef()->IsA<UCommonEquippableItemDefinition>())
	{
		if (UCommonEquipmentComponent* EquipmentComponent = OwnerActor->FindComponentByClass<UCommonEquipmentComponent>())
		{
			EquippedItems.Add(ContainerTag, ItemInstance);
			EquipmentComponent->EquipItem(ContainerTag, ItemInstance);

			//OPT: Notify listeners that the item has been equipped
		}
	}
}

void UCommonInventoryContainerComponent::UnequipItemInContainer(const FGameplayTag& ContainerTag, int32 ItemIndex)
{
	if (UCommonEquipmentComponent* EquipmentComponent = OwnerActor->FindComponentByClass<UCommonEquipmentComponent>())
	{
		if (UCommonInventoryItemInstance** ItemInstance = EquippedItems.Find(ContainerTag))
		{
			EquipmentComponent->UnequipItem(ContainerTag, *ItemInstance);
			EquippedItems.Remove(ContainerTag);

			//OPT: Notify listeners that the item has been unequipped
		}
	}
}

void UCommonInventoryContainerComponent::OnRep_ContainerList()
{
	FCommonInventoryContainerSlotsChangedMessage Message;
	Message.OwnerActor = OwnerActor;
	Message.AvatarActor = AvatarActor;

	TArray<FBlueprintContainerEntry> BPEntries;
	for (const auto& Entry : ContainerList.ContainerEntries)
	{
		FBlueprintContainerEntry& BPEntry = BPEntries.AddDefaulted_GetRef();
		BPEntry.Tag = Entry.Tag;
		BPEntry.Items = Entry.Items;
		BPEntry.ActiveItemIndex = Entry.ActiveItemIndex;
	}

	Message.ContainerEntries = BPEntries;

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(TAG_InventoryContainer_Message_SlotsChanged, Message);
}

void UCommonInventoryContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ContainerList, COND_None, REPNOTIFY_Always);
}
