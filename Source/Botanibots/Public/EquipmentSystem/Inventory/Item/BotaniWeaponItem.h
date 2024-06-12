// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item/CommonItem.h"
#include "BotaniWeaponItem.generated.h"

class UNiagaraSystem;
/*
 Weapon Fire Payload
 Stores data for the weapon fire event
 */
USTRUCT(BlueprintType)
struct FBotaniWeaponFirePayload
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Payload")
	bool bIsHitScan = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Payload")
	TArray<FHitResult> HitResults;

	bool IsValid() const
	{
		return HitResults.Num() > 0;
	}
};

/*
 Weapon Item
 Base class for all weapons
 */
UCLASS(Abstract, DisplayName = "Weapon Item", HideCategories = (Replication, Collision, Rendering, Input, HLOD, Physics, ActorTick, Actor, LevelInstance, DataLayers, Events, Cooking, WorldPartition, Tick))
class BOTANIBOTS_API ABotaniWeaponItem : public ACommonItem
{
	GENERATED_BODY()

public:
	ABotaniWeaponItem();

	//~ Begin ABotaniWeaponItem Contract
	virtual void FireWeapon(const FBotaniWeaponFirePayload& FirePayload);
	virtual void FireWeaponOutOfAmmo();
	virtual void SpawnImpactEffect(const FHitResult& HitResult);
	//~ End ABotaniWeaponItem Contract

	/** Effects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	bool bImpactEffectAttachedToHitActor = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<UNiagaraSystem>> ImpactEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	/** Sounds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> ReloadSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> OutOfAmmoSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> DestroyedSound;
	
	// Character
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Character")
	TObjectPtr<UAnimMontage> EquipAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Character")
	TObjectPtr<UAnimMontage> ReloadAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Character")
	TObjectPtr<UAnimMontage> PrimaryAbilityAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Character")
	TObjectPtr<UAnimMontage> SecondaryAbilityAnimation;

	// Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Weapon")
	TObjectPtr<UAnimMontage> WeaponEquipAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Weapon")
	TObjectPtr<UAnimMontage> WeaponReloadAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Weapon")
	TObjectPtr<UAnimMontage> WeaponPrimaryAbilityAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Weapon")
	TObjectPtr<UAnimMontage> WeaponSecondaryAbilityAnimation;
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor Interface

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;
};
