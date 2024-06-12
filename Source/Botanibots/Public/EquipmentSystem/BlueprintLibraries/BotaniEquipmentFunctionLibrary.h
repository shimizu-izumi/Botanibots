// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BotaniEquipmentFunctionLibrary.generated.h"

class ABotaniPickupProxy;

/**
 * 
 */
UCLASS()
class BOTANIBOTS_API UBotaniEquipmentFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/**
	 * Drop Item by Instance
	 * Removes the Item Instance from the Inventory and spawns it in the world.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory|Items", meta = (Displayname = "Drop Item (By Instance)", DefaultToSelf = "TargetActor", AdvancedDisplay = "Instigator, StackCount, OverridePickupActorClass"))
	static void DropItemFromSlot(AActor* TargetActor, APawn* Instigator, UCommonInventoryItemInstance* ItemToDrop, int32 StackCount = -1, TSubclassOf<ABotaniPickupProxy> OverridePickupActorClass = nullptr);
};
