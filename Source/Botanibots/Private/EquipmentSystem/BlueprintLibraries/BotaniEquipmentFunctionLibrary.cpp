// Copyright © 2024 Botanibots Team. All rights reserved.


#include "EquipmentSystem/BlueprintLibraries/BotaniEquipmentFunctionLibrary.h"

#include "BlueprintLibrary/CommonEquipmentBlueprintLibrary.h"
#include "Botanibots/Botanibots.h"
#include "GameplayTagStacks/Components/TagStackComponent.h"
#include "InteractionSystem/BotaniPickupProxy.h"
#include "Inventory/Component/CommonInventoryComponent.h"
#include "Inventory/Component/CommonInventoryContainerComponent.h"

void UBotaniEquipmentFunctionLibrary::DropItemFromSlot(AActor* TargetActor, APawn* Instigator, UCommonInventoryItemInstance* ItemToDrop, int32 StackCount, TSubclassOf<ABotaniPickupProxy> OverridePickupActorClass)
{
	// Check if we have a valid Item Instance
	if (!ItemToDrop)
	{
		UE_LOG(LogBotaniEquipment, Warning, TEXT("DropItemFromSlot: ItemToDrop is nullptr!"));
		return;
	}

	// Check if we have authority
	if (!TargetActor->HasAuthority())
	{
		UE_LOG(LogBotaniEquipment, Warning, TEXT("DropItemFromSlot: TargetActor is not Authority!"));
		return;
	}

	// Get the default pickup actor class if none is provided
	if (OverridePickupActorClass == nullptr)
	{
		OverridePickupActorClass = ABotaniPickupProxy::StaticClass();
	}

	// Try to get a valid Instigator
	if (Instigator == nullptr)
	{
		Instigator = Cast<APawn>(TargetActor);
	}

	// Check if we are dropping the entire stack
	const bool bDropEntireStack = StackCount == INDEX_NONE;
	int32 StackCountAfter = 0;

	// Spawn the pickup actor
	const FTransform SpawnTransform = FTransform(
		TargetActor->GetActorRotation(),
		TargetActor->GetActorLocation(),
		FVector::OneVector);

	ABotaniPickupProxy* SpawnedPickup = TargetActor->GetWorld()->SpawnActorDeferred<ABotaniPickupProxy>(
		OverridePickupActorClass,
		SpawnTransform,
		TargetActor,
		Instigator,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	// Finish spawning the pickup actor and store the tag stacks
	SpawnedPickup->bEnablePickupMovementOnBeginPlay = true;
	SpawnedPickup->FinishSpawning(SpawnTransform);

	if (UTagStackComponent* StackComponent = SpawnedPickup->FindComponentByClass<UTagStackComponent>())
	{
		StackComponent->StorePersistentStatTagStacks(ItemToDrop->GetStackContainer());
	}

	SpawnedPickup->OnPickupDropped(ItemToDrop->GetItemDef());

	if (UCommonInventoryComponent* InventoryComponent = UCommonEquipmentBlueprintLibrary::GetInventoryComponent(TargetActor))
	{
		InventoryComponent->RemoveItemInstance(ItemToDrop, StackCount, StackCountAfter);
	}

	if (bDropEntireStack || StackCountAfter <= 0)
	{
		if (UCommonInventoryContainerComponent* ContainerComponent = UCommonEquipmentBlueprintLibrary::GetInventoryContainerComponent(TargetActor))
		{
			const FGameplayTag ContainerTag = ContainerComponent->GetContainerTag(ItemToDrop);
			ContainerComponent->RemoveItemFromContainer(ContainerTag, ItemToDrop);
		}
	}
}
