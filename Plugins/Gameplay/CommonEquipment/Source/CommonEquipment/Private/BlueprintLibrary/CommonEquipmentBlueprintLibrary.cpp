// Copyright © 2024 Botanibots Team. All rights reserved.


#include "BlueprintLibrary/CommonEquipmentBlueprintLibrary.h"
#include "GameplayTagStacks/Components/TagStackComponent.h"
#include "Interfaces/EquipmentInterface.h"
#include "Inventory/Component/CommonEquipmentComponent.h"
#include "Inventory/Component/CommonInventoryComponent.h"
#include "Inventory/Component/CommonInventoryContainerComponent.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"

/*
UCommonInventoryItemInstance* UCommonEquipmentBlueprintLibrary::AddItemToInventoryByDefinition(
	AActor* TargetActor, AActor* Instigator, UCommonItemDefinition* InItemDefinition,
	int32 StackCount, FGameplayTag SlotTag, FGameplayTagContainer CreationContext, int32 SlotIndex)
{
	UCommonInventoryComponent* InventoryComponent = GetInventoryComponent(TargetActor);
	if (!InventoryComponent)
	{
		return nullptr;
	}

	const bool bEquipAfter = SlotIndex != INDEX_NONE;
	bool bIsNewEntry = false;

	// Add the item to the inventory
	UCommonInventoryItemInstance* Result = nullptr;
	if (InItemDefinition != nullptr)
	{
		Result = InventoryComponent->AddItemDefinition(InItemDefinition, StackCount, CreationContext, bIsNewEntry);

		// If we have an Instigator, check if there are any stored stacks
		if (Result && Instigator)
		{
			UTagStackComponent* TagStackComponent = Instigator->FindComponentByClass<UTagStackComponent>();
			if (TagStackComponent->HasAnyStacks())
			{
				for (const FGameplayTagStack& Stack : TagStackComponent->GetStackContainer().GetAllTagStacks())
				{
					Result->AddStatTagStack(Stack.GetStackTag(), Stack.GetStackValue().Magnitude, Stack.GetStackValue().bIsPersistent);
				}
			}
		}
	}

	// Equip the item if requested
	if (bEquipAfter && bIsNewEntry)
	{
		UCommonQuickBarComponent* QuickBarComponent = GetQuickBarComponent(TargetActor);
		if (!QuickBarComponent)
		{
			return Result;
		}

		QuickBarComponent->RemoveItemFromSlot(SlotTag, SlotIndex);
		QuickBarComponent->AddItemToSlot(SlotTag, Result);
		QuickBarComponent->SetActiveSlotIndex(SlotTag, SlotIndex);
	}

	return Result;
}
*/

UCommonInventoryComponent* UCommonEquipmentBlueprintLibrary::GetInventoryComponent(AActor* Target)
{
	UCommonInventoryComponent* InventoryComponent = nullptr;

	/** Tries to get the Inventory Component through the Equipment Interface */
	if (Target->Implements<UEquipmentInterface>())
	{
		InventoryComponent = Cast<IEquipmentInterface>(Target)->GetInventoryComponent();
	}

	/** If we still don't have an Inventory Component, try to get it through the Actor's Components */
	if (InventoryComponent == nullptr)
	{
		InventoryComponent = Target->FindComponentByClass<UCommonInventoryComponent>();
	}
	
	return InventoryComponent != nullptr ? InventoryComponent : nullptr;
}

UCommonEquipmentComponent* UCommonEquipmentBlueprintLibrary::GetEquipmentComponent(AActor* Target)
{
	UCommonEquipmentComponent* EquipmentComponent = nullptr;

	/** Tries to get the Equipment Component through the Equipment Interface */
	if (Target->Implements<UEquipmentInterface>())
	{
		EquipmentComponent = Cast<IEquipmentInterface>(Target)->GetEquipmentComponent();
	}

	/** If we still don't have an Equipment Component, try to get it through the Actor's Components */
	if (EquipmentComponent == nullptr)
	{
		EquipmentComponent = Target->FindComponentByClass<UCommonEquipmentComponent>();
	}

	return EquipmentComponent != nullptr ? EquipmentComponent : nullptr;
}

UCommonInventoryContainerComponent* UCommonEquipmentBlueprintLibrary::GetInventoryContainerComponent(AActor* Target)
{
	UCommonInventoryContainerComponent* InventoryContainerComponent = nullptr;

	/** Tries to get the Inventory Container Component through the Equipment Interface */
	if (Target->Implements<UEquipmentInterface>())
	{
		InventoryContainerComponent = Cast<IEquipmentInterface>(Target)->GetInventoryContainerComponent();
	}

	/** If we still don't have an Inventory Container Component, try to get it through the Actor's Components */
	if (InventoryContainerComponent == nullptr)
	{
		InventoryContainerComponent = Target->FindComponentByClass<UCommonInventoryContainerComponent>();
	}

	return InventoryContainerComponent != nullptr ? InventoryContainerComponent : nullptr;
}

UCommonInventoryItemInstance* UCommonEquipmentBlueprintLibrary::AddItemToInventoryByDefinition(AActor* TargetActor,
	AActor* Instigator, UCommonItemDefinition* InItemDefinition, int32 StackCount, FGameplayTag ContainerTag,
	FGameplayTagContainer CreationContext, int32 SlotIndex)
{
	return nullptr;
}
