// Copyright © 2024 MajorT. All rights reserved.


#include "Inventory/Component/CommonEquipmentComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Inventory/Definition/CommonEquippableItemDefinition.h"
#include "Inventory/Fragments/ItemFragments/ItemFragment_Abilities.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"
#include "Inventory/Item/CommonItem.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonEquipmentComponent)

/************************************************************************************************************************/
/* FCommonEquipmentList																									*/
/************************************************************************************************************************/

class UCommonEquippableItemDefinition;
class UItemFragment_Abilities;

void FCommonEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const int32 Index : RemovedIndices)
	{
		const FCommonAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.ItemInstance != nullptr)
		{
			Entry.ItemInstance->OnUnequipped();
		}
	}
}

void FCommonEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 Index : AddedIndices)
	{
		const FCommonAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.ItemInstance != nullptr)
		{
			Entry.ItemInstance->OnEquipped();
		}
	}
}

void FCommonEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	// Nope
}

void FCommonEquipmentList::AddEntry(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* Instance)
{
	check(Instance);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	FCommonAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.ItemInstance = Instance;

	// Grant Items
	if (UCommonEquipmentComponent* EquipmentComponent = Cast<UCommonEquipmentComponent>(OwnerComponent))
	{
		EquipmentComponent->SpawnEquipmentActors(ContainerTag, Instance ,Cast<ACharacter>(EquipmentComponent->AvatarActor)->GetMesh());
	}

	// Grant Abilities
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerComponent->GetOwner()))
	{
		if (const auto& AbilityItemFrag = Instance->GetItemDef()->GetItemFragment<UItemFragment_Abilities>())
		{
			AbilityItemFrag->AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Instance);
		}
	}

	MarkItemDirty(NewEntry);
}

void FCommonEquipmentList::RemoveEntry(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* Instance)
{
	for (auto EntryIterator = Entries.CreateIterator(); EntryIterator; ++EntryIterator)
	{
		FCommonAppliedEquipmentEntry& Entry = *EntryIterator;
		if (Entry.ItemInstance == Instance)
		{
			// Remove Granted Items
			if (UCommonEquipmentComponent* EquipmentComponent = Cast<UCommonEquipmentComponent>(OwnerComponent))
			{
				EquipmentComponent->DestroyEquipmentActors(ContainerTag);
			}
			
			// Remove Granted Abilities
			if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerComponent->GetOwner()))
			{
				if (const auto& AbilityItemFrag = Instance->GetItemDef()->GetItemFragment<UItemFragment_Abilities>())
				{
					AbilityItemFrag->AbilitySet->TakeFromAbilitySystem(ASC, &Entry.GrantedHandles);
				}
			}

			EntryIterator.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

/************************************************************************************************************************/
/* UCommonEquipmentComponent																							*/
/************************************************************************************************************************/

UCommonEquipmentComponent::UCommonEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UCommonEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UCommonEquipmentComponent, EquipmentList);
	DOREPLIFETIME_CONDITION_NOTIFY(UCommonEquipmentComponent, SpawnedActors, COND_None, REPNOTIFY_Always);
}

void UCommonEquipmentComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UCommonEquipmentComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
}

void UCommonEquipmentComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
}

void UCommonEquipmentComponent::EquipItem(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr)
	{
		return;
	}
	EquipmentList.AddEntry(ContainerTag, ItemInstance);
}

void UCommonEquipmentComponent::UnequipItem(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr)
	{
		return;
	}
	EquipmentList.RemoveEntry(ContainerTag, ItemInstance);
}

void UCommonEquipmentComponent::SpawnEquipmentActors(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance, USceneComponent* AttachmentTargetOverride)
{
	APawn* OwningPawn = Cast<APawn>(AvatarActor);
	if (!OwningPawn)
	{
		return;
	}

	// If we don't override the attachment target, use the root component of the owning pawn
	if (!AttachmentTargetOverride)
	{
		AttachmentTargetOverride = OwningPawn->GetRootComponent();
	}

	// Spawn the equipment Actor and attach it to the owner
	if (const UCommonEquippableItemDefinition* EquippableItemDefinition = Cast<UCommonEquippableItemDefinition>(ItemInstance->GetItemDef()))
	{
		const FCommonEquipmentData& EquipmentData = EquippableItemDefinition->GetEquipmentData();

		TSubclassOf<AActor> ActorClass = EquipmentData.EquipmentActor.LoadSynchronous();
		checkf(ActorClass, TEXT("Equipment Actor is not set on Equippable Item Definition %s"), *ItemInstance->GetItemDef()->GetName());


		// Spawn the Item Actor
		AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(ActorClass, FTransform::Identity, OwningPawn);
		NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
		NewActor->SetActorRelativeTransform(EquipmentData.RelativeTransform);
		NewActor->AttachToComponent(AttachmentTargetOverride, FAttachmentTransformRules::KeepRelativeTransform, EquipmentData.AttachmentSocket);

		// Notify the Item that it has been equipped
		if (ACommonItem* Item = Cast<ACommonItem>(NewActor))
		{
			Item->OnEquipped(OwningPawn);
		}
		
		SpawnedActors.Add(FSpawnedActorInfo(ContainerTag, NewActor));
	}
}

void UCommonEquipmentComponent::DestroyEquipmentActors(const FGameplayTag& ContainerTag)
{
	for (auto EntryIterator = SpawnedActors.CreateIterator(); EntryIterator; ++EntryIterator)
	{
		FSpawnedActorInfo& Info = *EntryIterator;
		
		if (Info.Actor && Info.ContainerTag.MatchesTagExact(ContainerTag))
		{
			if (ACommonItem* Item = Cast<ACommonItem>(Info.Actor))
			{
				Item->OnUnequipped();
			}

			Info.Actor->Destroy();
			EntryIterator.RemoveCurrent();
		}
	}
}

void UCommonEquipmentComponent::OnRep_SpawnedActors()
{
}
