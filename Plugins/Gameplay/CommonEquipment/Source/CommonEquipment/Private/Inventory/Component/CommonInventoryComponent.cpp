// Copyright © 2024 MajorT. All rights reserved.


#include "Inventory/Component/CommonInventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CommonEquipment.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Inventory/Definition/CommonItemDefinition.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "NativeGameplayTags.h"
#include "Inventory/Component/CommonInventoryContainerComponent.h"
#include "Inventory/Data/InventoryConfig.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonInventoryComponent)

FCommonInventoryEntry FCommonInventoryEntry::EmptyEntry = FCommonInventoryEntry(nullptr, INDEX_NONE);
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Inventory_Message_StackChanged, "Inventory.Message.StackChanged");

/************************************************************************************************************************/
/* FCommonInventoryList																									*/
/************************************************************************************************************************/

void FCommonInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const int32 i : RemovedIndices)
	{
		FCommonInventoryEntry& Entry = Entries[i];
		BroadcastChangeMessage(Entry, Entry.StackCount, 0);
		Entry.LastObservedStackCount = 0;
	}
}

void FCommonInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 i : AddedIndices)
	{
		FCommonInventoryEntry& Entry = Entries[i];
		BroadcastChangeMessage(Entry, 0, Entry.StackCount);
		Entry.LastObservedStackCount = Entry.StackCount;
	}
}

void FCommonInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const int32 i : ChangedIndices)
	{
		FCommonInventoryEntry& Entry = Entries[i];
		checkf(Entry.LastObservedStackCount != INDEX_NONE, TEXT("LastObservedStackCount is INDEX_NONE"));
		BroadcastChangeMessage(Entry, Entry.LastObservedStackCount, Entry.StackCount);
		Entry.LastObservedStackCount = Entry.StackCount;
	}
}

void FCommonInventoryList::BroadcastChangeMessage(FCommonInventoryEntry& Entry, int32 OldCount, int32 NewCount) const
{
	FCommonInventoryChangedMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSubsystem.BroadcastMessage(TAG_Inventory_Message_StackChanged, Message);
}

TArray<UCommonInventoryItemInstance*> FCommonInventoryList::GetAllItems() const
{
	TArray<UCommonInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());

	for (const FCommonInventoryEntry& Entry : Entries)
	{
		if (Entry.IsValid())
		{
			Results.Add(Entry.Instance);
		}
	}

	return Results;
}

FCommonInventoryEntry& FCommonInventoryList::GetEntry(UCommonInventoryItemInstance* ItemInstance)
{
	for (FCommonInventoryEntry& Entry : Entries)
	{
		if (!Entry.IsValid())
		{
			continue;
		}

		if (Entry.Instance == ItemInstance)
		{
			return Entry;
		}
	}
	return FCommonInventoryEntry::EmptyEntry;
}

FCommonInventoryEntry& FCommonInventoryList::GetEntry(UCommonItemDefinition* ItemDefinition)
{
	for (FCommonInventoryEntry& Entry : Entries)
	{
		if (!Entry.IsValid())
		{
			continue;
		}
		
		if (Entry.Instance->GetItemDef() == ItemDefinition)
		{
			return Entry;
		}
	}
	return FCommonInventoryEntry::EmptyEntry;
}

UCommonInventoryItemInstance* FCommonInventoryList::AddEntry(UCommonItemDefinition* ItemDefinition, int32 StackCount, bool& bIsNewEntry)
{
	return AddEntry(ItemDefinition, StackCount, FGameplayTagContainer(), bIsNewEntry);
}

UCommonInventoryItemInstance* FCommonInventoryList::AddEntry(UCommonItemDefinition* ItemDefinition, int32 StackCount, FGameplayTagContainer CreationContext, bool& bIsNewEntry)
{
	// Validate the inputs and the context
	checkf(OwnerComponent != nullptr, TEXT("OwnerComponent is nullptr in: [%hs]"), __FUNCTION__);
	checkf(ItemDefinition != nullptr, TEXT("ItemDefinition in: [%s] is nullptr in: [%hs]"), *OwnerComponent->GetFullName(), __FUNCTION__);
	AActor* OwningActor = OwnerComponent->GetOwner();
	checkf(OwningActor->HasAuthority(), TEXT("OwningActor: [%s] does not have authority in: [%hs]"), *OwningActor->GetFullName(), __FUNCTION__);

	UCommonInventoryItemInstance* Result = nullptr;

	if (ItemDefinition->EquipmentRequirements.StackingData.bSupportsStacking)
	{
		for (FCommonInventoryEntry& Entry : Entries)
		{
			if (!Entry.IsValid())
			{
				continue;
			}
		
			if (Entry.Instance->GetItemDef()->ItemTag.MatchesTagExact(ItemDefinition->ItemTag))
			{
				// We found an existing entry for this item
				const FCommonItemStackingData StackingData = Entry.Instance->GetItemDef()->EquipmentRequirements.StackingData;

				// Check if we found an Entry that can still be stacked
				if (!StackingData.bSupportsStacking || Entry.StackCount >= StackingData.MaxStackSize)
				{
					continue;
				}

				// Calculate the total stack size
				int32 TotalStackSize = Entry.StackCount + StackCount;
				StackCount = TotalStackSize - StackingData.MaxStackSize;
				TotalStackSize = FMath::Min(TotalStackSize, StackingData.MaxStackSize);

				// Update the Entry
				Entry.StackCount = TotalStackSize;
				Result = Entry.Instance;
				bIsNewEntry = false;
				BroadcastChangeMessage(Entry, Entry.StackCount - StackCount, Entry.StackCount);
				
				// Mark the item dirty so that it will be replicated the next net update
				MarkItemDirty(Entry);

				// If we have added all the items, we can return
				if (StackCount <= 0)
				{
					return Result;
				}
				break;
			}
		}
	}

	if (StackCount > 0)
	{
		// Otherwise, create a new entry and add it to the list
		FCommonInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Instance = NewObject<UCommonInventoryItemInstance>(OwningActor, ItemDefinition->GetItemInstanceClass());
		NewEntry.Instance->SetItemDef(ItemDefinition);
		NewEntry.Instance->SetOwnerComponent(OwnerComponent);
		NewEntry.StackCount = StackCount;
		Result = NewEntry.Instance;
		bIsNewEntry = true;

		// Notify all Item Fragments that the instance has been created
		for (UCommonItemFragment* ItemFragment : ItemDefinition->ItemFragments)
		{
			// Check that the Item Fragment is valid for the context
			if (ItemFragment->IsFragmentValidForContext(CreationContext))
			{
				ItemFragment->OnInstanceCreated(NewEntry.Instance, CreationContext);	
			}
		}
		
		NewEntry.Instance->OnInstanceCreated(ItemDefinition, OwnerComponent, CreationContext);

		// Mark the item dirty so that it will be replicated the next net update
		MarkItemDirty(NewEntry);	
	}

	return Result;
}

void FCommonInventoryList::RemoveEntry(UCommonInventoryItemInstance* ItemInstance, int32 StackCount, int32& StackCountAfter)
{
	const bool bRemoveEntireStack = StackCount == INDEX_NONE;

	for (auto EntryIterator = Entries.CreateIterator(); EntryIterator; ++EntryIterator)
	{
		FCommonInventoryEntry& Entry = *EntryIterator;
		if (Entry.Instance == ItemInstance)
		{
			// Remove the entry
			if (bRemoveEntireStack)
			{
				EntryIterator.RemoveCurrent();
				StackCountAfter = 0;
				MarkArrayDirty();
			}
			else
			{
				// Remove the specified number of items from the stack
				Entry.StackCount -= StackCount;
				StackCountAfter = Entry.StackCount;

				// If the stack is now empty, remove the entry
				if (StackCountAfter<= 0)
				{
					EntryIterator.RemoveCurrent();
					MarkArrayDirty();
				}
			}
		}
	}
}

/************************************************************************************************************************/
/* UCommonInventoryComponent																							*/
/************************************************************************************************************************/

UCommonInventoryComponent::UCommonInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UCommonInventoryComponent::InitInventory(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitInventory(InOwnerActor, InAvatarActor);

	/** Setup any other inventory configurations */
	for (UInventoryConfigFragment* Config : InventoryConfig->InventoryConfigurations)
	{
		Config->PostInventoryInitialize(this, InOwnerActor, InAvatarActor);
	}

	/** Give any initial items to the inventory */
	for (const FCommonInitialItemConfig& InitialItem : InventoryConfig->InitialItems)
	{
		bool bIsNewEntry;
		UCommonInventoryItemInstance* NewItem = AddItemDef(InitialItem.Item, InitialItem.Count, FGameplayTagContainer(), bIsNewEntry);
		UE_LOG(LogCommonEquipment, Log, TEXT("Added Item: [%s] to Inventory: [%s]"), *InitialItem.Item->GetName(), *GetName());

		/** Add the item to a container if its possible and if its set to do so */
		if (bIsNewEntry && IsValid(NewItem) && InitialItem.bAddToContainer)
		{
			UCommonInventoryContainerComponent* ContainerComponent = OwnerActor->GetComponentByClass<UCommonInventoryContainerComponent>();

			if (ContainerComponent == nullptr)
			{
				UE_LOG(LogCommonEquipment, Error, TEXT("OwnerActor: [%s] does not have a UCommonInventoryContainerComponent"), *OwnerActor->GetName());
				return;
			}

			const int32 NewItemIndex = ContainerComponent->AddItemToContainer(InitialItem.ContainerTag, NewItem);

			// Equip the item if it's set to do so
			if (InitialItem.bEquipItem)
			{
				ContainerComponent->SetActiveItemInContainer(InitialItem.ContainerTag, NewItemIndex);
				UE_LOG(LogCommonEquipment, Log, TEXT("Item: [%s] was added to the container: [%s] with index: [%d]"), *InitialItem.Item->GetName(), *ContainerComponent->GetName(), NewItemIndex);
			}
		}
		else
		{
			UE_LOG(LogCommonEquipment, Log , TEXT("Item: [%s] was not added to the container"), *InitialItem.Item->GetName());
		}
	}
}

void UCommonInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, InventoryList);
	DOREPLIFETIME(ThisClass, ChildComponents);
}


bool UCommonInventoryComponent::CanAddItemDef(UCommonItemDefinition* ItemDefinition, int32 StackCount) const
{
	const FGameplayTagContainer TargetRequiredTags	= ItemDefinition->EquipmentRequirements.TargetRequiredTags;
	const FGameplayTagContainer TargetBlockedTags	= ItemDefinition->EquipmentRequirements.TargetBlockedTags;
	const FGameplayTagContainer RequiredItemTags	= ItemDefinition->EquipmentRequirements.RequiredItemTags;
	const FGameplayTagContainer BlockedItemTags		= ItemDefinition->EquipmentRequirements.BlockedItemTags;

	if (const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AvatarActor))
	{
		// If we have any of the blocked tags, we can't add the item
		if (ASC->HasAnyMatchingGameplayTags(TargetBlockedTags))
		{
			return false;
		}

		// If we don't have all the required tags, we can't add the item
		if (!ASC->HasAllMatchingGameplayTags(TargetRequiredTags))
		{
			return false;
		}
	}

	FGameplayTagContainer AllItemTags;
	for (const UCommonInventoryItemInstance* ItemInstance : InventoryList.GetAllItems())
	{
		const UCommonItemDefinition* ItemDef = ItemInstance->GetItemDef();

		// If we have any of the items that are blocked by this item, we can't add the item
		if (BlockedItemTags.HasTagExact(ItemDef->ItemTag))
		{
			return false;
		}

		AllItemTags.AddTag(ItemDef->ItemTag);
	}

	// If we don't have all the required item tags, we can't add the item
	if (!AllItemTags.HasAllExact(RequiredItemTags))
	{
		return false;
	}
	return true;
}

UCommonInventoryItemInstance* UCommonInventoryComponent::AddItemDef(UCommonItemDefinition* ItemDefinition, int32 StackCount, FGameplayTagContainer CreationContext, bool& bIsNewEntry)
{
	checkf(ItemDefinition != nullptr, TEXT("ItemDefinition is nullptr in; [%hs]"), __FUNCTION__);

	// Make sure we can add the item
	if (!CanAddItemDef(ItemDefinition, StackCount))
	{
		bIsNewEntry = false;
		return nullptr;
	}

	UCommonInventoryItemInstance* Result  = InventoryList.AddEntry(ItemDefinition, StackCount, CreationContext, bIsNewEntry);

	// If we added a new item, we need to register the item for replication
	if (bIsNewEntry)
	{
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}

	return Result;
}

void UCommonInventoryComponent::RemoveItemInstance(UCommonInventoryItemInstance* ItemInstance, int32 StackCount, int32& StackCountAfter)
{
	if (!IsValid(ItemInstance))
	{
		StackCountAfter = INDEX_NONE;
		return;
	}

	InventoryList.RemoveEntry(ItemInstance, StackCount, StackCountAfter);

	// If we removed the last item, we need to unregister the item for replication
	if (StackCountAfter <= 0)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}
	}
}

void UCommonInventoryComponent::AddChildComponent(UCommonEquipmentBaseComponent* ChildComponent)
{
	if (ChildComponent)
	{
		ChildComponent->InitInventory(OwnerActor, AvatarActor);
		ChildComponents.Add(ChildComponent);
	}
}

bool UCommonInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FCommonInventoryEntry& Entry : InventoryList.Entries)
	{
		UCommonInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void UCommonInventoryComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register all existing Inventory Item Instances for replication
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FCommonInventoryEntry& Entry : InventoryList.Entries)
		{
			if (UCommonInventoryItemInstance* Instance = Entry.Instance; IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

void UCommonInventoryComponent::OnUnregister()
{
	Super::OnUnregister();
	ChildComponents.Empty();
}

UCommonEquipmentBaseComponent* UCommonInventoryComponent::K2_GetInventoryChildComponent(TSubclassOf<UCommonEquipmentBaseComponent> ComponentClass)
{
	for (UCommonEquipmentBaseComponent* ChildComponent : ChildComponents)
	{
		if (ChildComponent->IsA(ComponentClass))
		{
			return ChildComponent;
		}
	}

	return nullptr;
}

void UCommonInventoryComponent::OnRep_ChildComponents()
{
}
