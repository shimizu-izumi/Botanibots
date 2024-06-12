// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonEquipmentBlueprintLibrary.generated.h"

struct FGameplayTagContainer;
class UCommonInventoryContainerComponent;
struct FGameplayTagStackContainer;
class UCommonItemDefinition;
class UCommonInventoryItemInstance;
class UCommonEquipmentComponent;
class UCommonInventoryComponent;
class UTagStackComponent;

/**
 * 
 */
UCLASS()
class COMMONEQUIPMENT_API UCommonEquipmentBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/***********************************************************************************************************************
	* COMPONENT GETTER
	**********************************************************************************************************************/
	
	/**
	 * Tries to find a UCommonInventoryComponent on the Target Actor
	 * Will try it through the Interface first, then through the Actor's Components
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Components", meta = (DisplayName = "Get Inventory Component", DefaultToSelf = "Target"))
	static UCommonInventoryComponent* GetInventoryComponent(AActor* Target);

	/**
	 * Tries to find a UCommonEquipmentComponent on the Target Actor
	 * Will try it through the Interface first, then through the Actor's Components
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Components", meta = (DisplayName = "Get Equipment Component", DefaultToSelf = "Target"))
	static UCommonEquipmentComponent* GetEquipmentComponent(AActor* Target);

	/**
	 * Tries to find a UCommonInventoryContainerComponent on the Target Actor
	 * Will try it through the Interface first, then through the Actor's Components
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Components", meta = (DisplayName = "Get Inventory Container Component", DefaultToSelf = "Target"))
	static UCommonInventoryContainerComponent* GetInventoryContainerComponent(AActor* Target);

	/**
	 * Get Tag Stack Component
	 */
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Components", meta = (DisplayName = "Get Tag Stack Component", DefaultToSelf = "Target"))
	//static UTagStackComponent* GetTagStackComponent(AActor* Target);
	

	/***********************************************************************************************************************
	 * INVENTORY SYSTEM
	 **********************************************************************************************************************/ 

	/**
	 * Add Item by Definition
	 * Creates a new Item Instance based on the Item Definition and adds it to the Inventory.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory|Items", meta = (DisplayName = "Add Item to Inventory (By Definition)", DefaultToSelf = "TargetActor", AdvancedDisplay = "Instigator, SlotIndex, StackCount, ContainerTag"))
	static UPARAM(DisplayName = "Item Instance") UCommonInventoryItemInstance* AddItemToInventoryByDefinition(AActor* TargetActor, AActor* Instigator, UCommonItemDefinition* InItemDefinition, int32 StackCount = 1, FGameplayTag ContainerTag = FGameplayTag(), FGameplayTagContainer CreationContext = FGameplayTagContainer(), int32 SlotIndex = -1);

	/***********************************************************************************************************************
	 * TAG STACK SYSTEM
	 **********************************************************************************************************************/
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|TagStack", meta = (DisplayName = "Get Stack Tag"))
	//static FGameplayTag GetStackTag(FGameplayTagStack Stack) { return Stack.GetStackTag(); }

	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|TagStack", meta = (DisplayName = "Get Stack Magnitude"))
	//static float GetStackMagnitude(FGameplayTagStack Stack) { return Stack.GetStackValue().Magnitude; }

	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|TagStack", meta = (DisplayName = "Get Stack Is Persistent"))
	//static bool GetStackIsPersistent(FGameplayTagStack Stack) { return Stack.GetStackValue().bIsPersistent; }

	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|TagStack", meta = (DisplayName = "Get Stack Value"))
	//static FGameplayTagStackValue GetStackValue(FGameplayTagStack Stack) { return Stack.GetStackValue(); }
};
