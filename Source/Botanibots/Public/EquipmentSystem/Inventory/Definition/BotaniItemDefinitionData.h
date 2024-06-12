// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BotaniItemDefinitionData.generated.h"

/*
 Item Element Data
 */
USTRUCT(BlueprintType)
struct FBotaniElementData : public FTableRowBase
{
	GENERATED_BODY()

	/*
	 Element Tag
	 Maps to the element tag of the item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Element", meta = (Categories = "Element.Type"))
	FGameplayTag ElementTag;

	/*
	 Element Color
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Element")
	FLinearColor ElementColor = FLinearColor::White;
};

/*
 Weapon Spread data
 Stores data for the spread of a weapon
 */
USTRUCT(BlueprintType)
struct FBotaniWeaponSpreadData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data|Stats")
	float Spread = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data|Stats", meta=(ForceUnits=x))
	float StandingStillSpreadMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data|Stats", meta=(ForceUnits=x))
	float CrouchingSpreadMultiplier = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data|Stats", meta=(ForceUnits=x))
	float SprintingSpreadMultiplier = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data|Stats")
	float SpreadExponent = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data|Stats", meta=(ForceUnits=s))
	float SpreadRecoveryCooldown = 1.0f;
};

/*
 Weapon Stats
 Stores data for the stats of a weapon
 */
USTRUCT(BlueprintType)
struct FBotaniWeaponStatData : public FTableRowBase
{
	GENERATED_BODY()

	/*
	 Bullets per Cartridge
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data|Ammo")
	uint8 BulletsPerCartridge = 1;

	/*
	 Ammo Cost per Fire
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data|Ammo")
	uint8 AmmoCostPerFire = 1;
	
	/*
	 Firing Rate
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data|Firing", meta = (ForceUnits=RevolutionsPerMinute))
	float FiringRate = 1.0f;

	/*
	 Sweep Radius
	 Defines the radius of the sweep for the weapon
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data|Firing", meta = (ForceUnits=cm))
	float SweepRadius = -1.f;

	/*
	 Spread Data
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data|Firing")
	FBotaniWeaponSpreadData SpreadData;
};
