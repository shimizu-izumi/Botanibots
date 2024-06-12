// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Definition/CommonEquippableItemDefinition.h"
#include "BotaniWeaponDefinition.generated.h"

class UBotaniWeaponFragment;
/*
 Reticle Widget Data
 Stores data for a single Reticle of the weapon
 */
USTRUCT(BlueprintType)
struct FBotaniReticleWidgetData
{
    GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Reticle")
	TSoftClassPtr<UUserWidget> ReticleWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Reticle", meta = (DisplayName = "HUD Extension Point Tag", Categories = "HUD.Slot"))
	FGameplayTag HUDExtensionPointTag = FGameplayTag::EmptyTag;
};

/*
 Reticle Data
 Stores data about all Reticles and other reticle related data
 */
USTRUCT(BlueprintType)
struct FBotaniReticleData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Reticle")
	TArray<FBotaniReticleWidgetData> ReticleWidgets;


	UPROPERTY(EditDefaultsOnly, Category = "Reticle")
	bool bShotHitNotification = true;

	UPROPERTY(EditDefaultsOnly, Category = "Reticle", meta = (EditCondition = "bShotHitNotification"))
	float HitNotificationDuration = 0.5f;
};

/*
 * 
 */
UCLASS()
class BOTANIBOTS_API UBotaniWeaponDefinition : public UCommonEquippableItemDefinition
{
	GENERATED_BODY()

public:

	/*
	 Allow Secondary Fire to interrupt Primary Fire
	 Sets if any secondary fire action should be able to interrupt or cancel the primary fire action
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
	bool bAllowSecondaryFireToInterruptPrimaryFire = true;

	/*
	 Auto Reload
	 Sets if the weapon should automatically reload when out of ammo
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
	bool bAutoReload = false;

	/*
	 Targeting Prevents Reload
	 Sets if the weapon should prevent reloading while targeting
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
	bool bTargetingPreventsReload = false;

	/*
	 Reticle Data
	 Stores data about all Reticles and other reticle related data
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reticle")
	FBotaniReticleData ReticleData;

	/*
	 Weapon Fragments
	 List of Weapon Fragments that make up this weapon
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fragments")
	TArray<TObjectPtr<UBotaniWeaponFragment>> WeaponTypes;
	
};
