// Copyright © 2024 Botanibots Team. All rights reserved.


#include "EquipmentSystem/Component/BotaniEquipmentComponent.h"

#include "Characters/BotaniPlayer.h"
#include "Interfaces/BotaniCombatInterface.h"
#include "Inventory/Definition/CommonEquippableItemDefinition.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"
#include "Inventory/Item/CommonItem.h"

UBotaniEquipmentComponent::UBotaniEquipmentComponent()
{
}

void UBotaniEquipmentComponent::SpawnEquipmentActors(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance, USceneComponent* AttachmentTargetOverride)
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
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


		// Spawn the Third Person Mesh Actor
		AActor* NewActorTP = GetWorld()->SpawnActorDeferred<AActor>(ActorClass, FTransform::Identity, OwningPawn, OwningPawn);
		NewActorTP->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
		NewActorTP->SetActorRelativeTransform(EquipmentData.RelativeTransform);
		NewActorTP->AttachToComponent(AttachmentTargetOverride, FAttachmentTransformRules::KeepRelativeTransform, EquipmentData.AttachmentSocket);

		// Notify the Item that it has been equipped and setup visibility
		if (ACommonItem* Item = Cast<ACommonItem>(NewActorTP))
		{
			Item->OnEquipped(OwningPawn);
		}

		// Cache the spawned actor
		SpawnedActors.Add(FSpawnedActorInfo(ContainerTag, NewActorTP));

		// Spawn the First Person Mesh Actor
		if (const IBotaniCombatInterface* CombatPtr = Cast<IBotaniCombatInterface>(OwningPawn))
		{
			AttachmentTargetOverride = CombatPtr->Execute_GetFirstPersonMesh(OwningPawn);
			AActor* NewActorFP = GetWorld()->SpawnActorDeferred<AActor>(ActorClass, FTransform::Identity, OwningPawn);
			NewActorFP->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
			NewActorFP->SetActorRelativeTransform(EquipmentData.RelativeTransform);
			NewActorFP->AttachToComponent(AttachmentTargetOverride, FAttachmentTransformRules::KeepRelativeTransform, EquipmentData.AttachmentSocket);

			// Notify the Item that it has been equipped and setup visibility
			if (ACommonItem* Item = Cast<ACommonItem>(NewActorFP))
			{
				Item->OnEquipped(OwningPawn);
			}

			// Cache the spawned actor
			SpawnedActors.Add(FSpawnedActorInfo(ContainerTag, NewActorFP));
		}

		OnRep_SpawnedActors();
	}
}

void UBotaniEquipmentComponent::OnRep_SpawnedActors()
{
	Super::OnRep_SpawnedActors();
}
