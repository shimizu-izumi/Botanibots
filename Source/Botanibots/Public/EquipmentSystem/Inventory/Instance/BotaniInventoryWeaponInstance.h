// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"
#include "BotaniInventoryWeaponInstance.generated.h"

class UBotaniWeaponFragment;
/*
 Weapon Inventory Item Instance
 An Instance of a Weapon Inventory Item
 */
UCLASS()
class BOTANIBOTS_API UBotaniInventoryWeaponInstance : public UCommonInventoryItemInstance
{
	GENERATED_BODY()

public:
	//~Begin UCommonInventoryItemInstance Contract
	virtual void OnInstanceCreated(UCommonItemDefinition* ItemDef, UActorComponent* OwningComponent, FGameplayTagContainer CreationContext) override;
	//~End UCommonInventoryItemInstance Contract

	/*
	 Get the Weapon Definition for this Weapon Inventory Item Instance
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Item", meta = (DeterminesOutputType = "WeaponClass"))
	UBotaniWeaponFragment* GetWeaponDefinitionByClass(TSubclassOf<UBotaniWeaponFragment> WeaponClass) const;
};
