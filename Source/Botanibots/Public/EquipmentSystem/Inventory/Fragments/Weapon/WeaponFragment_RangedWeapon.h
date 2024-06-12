// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSystem/Inventory/Fragments/BotaniWeaponFragment.h"
#include "WeaponFragment_RangedWeapon.generated.h"

class ABotaniProjectileBase;
class UBotaniWeaponAmmoDefinition;


/*
 Ranged Weapon Fragment
 Weapon Fragment for Ranged Weapons
 */
UCLASS(DisplayName = "Ranged Weapon")
class BOTANIBOTS_API UWeaponFragment_RangedWeapon : public UBotaniWeaponFragment
{
	GENERATED_BODY()

public:
	UWeaponFragment_RangedWeapon();
	
	//~Begin UCommonFragment Contract
	virtual void OnInstanceCreated(UCommonInventoryItemInstance* ItemInstance, const FGameplayTagContainer& CreationContext) override;
	//~End UCommonFragment Contract

	/***********************************************************************************************************************
	* ITEM PROPERTIES
	**********************************************************************************************************************/

	/*
	 Projectile Template
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data|Ammo")
	TSoftClassPtr<ABotaniProjectileBase> ProjectileTemplate;

	/*
	 Ammo Data
	 */
	UPROPERTY(EditDefaultsOnly	, BlueprintReadOnly, Category = "Weapon Data|Ammo")
	TObjectPtr<UBotaniWeaponAmmoDefinition> AmmoData;

	/*
	 Low Ammo Percentage
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data|Ammo", meta = (ForceUnits="%", ClampMin=0.0, ClampMax=100.0, UIMin=0.0, UIMax=100.0))
	float LowAmmoPercentage = 25.f;

	/*
	 Show Ammo as Percentage
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data|Ammo")
	bool bShowAmmoAsPercentage = false;

	/*
	 Weapon Stats Data Table
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data", meta = (RowType = "/Script/Botanibots.BotaniWeaponStatData"))
	FDataTableRowHandle WeaponStats;
};
