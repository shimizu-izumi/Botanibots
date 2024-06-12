// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BotaniProjectileBase.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class USphereComponent;
class UNiagaraSystem;
/*
 Damage Sense Type
 Describes how the Hit Target should react to the damage
 */
UENUM(BlueprintType)
enum class EBotaniDamageSenseType : uint8
{
	/*
	 Environmental
	 The Hit target wont be provoked by the damage
	 */
	Environmental,
	/*
	 Combat
	 The Hit target will be provoked by the damage if its set to be so 
	 */
	Combat,
};

/*
 Projectile Base
 The Base class for all Projectiles
 */
UCLASS(Abstract, HideCategories = (Replication, Input, HLOD, DataLayers, WorldPartition, Tick, Physics, LevelInstance, Cooking, Actor, Collision))
class BOTANIBOTS_API ABotaniProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	ABotaniProjectileBase();

	//~Begin ABotaniProjectileBase Contract
	virtual void ExplodeProjectile();
	//~End ABotaniProjectileBase Contract
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|VFX")
	TObjectPtr<UNiagaraSystem> ProjectileRibbonSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|VFX")
	TObjectPtr<UNiagaraSystem> ProjectileExplosionSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Fuse", meta = (DisplayName = "Fuse Timer after Spawn"))
	float FuseTime = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Fuse")
	float MaxProjectileLifeTime = 40.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Fuse")
	float ExplodeRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Fuse")
	TObjectPtr<UCurveFloat> ExplodeDamageFalloff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Damage")
	EBotaniDamageSenseType DamageSenseType = EBotaniDamageSenseType::Environmental;

protected:
	//~Begin AActor Contract
	virtual void BeginPlay() override;
	//~End AActor Contract

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> ProjectileHitbox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> ProjectileEnvironmentHitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> ProjectileRibbonComponent;

	UFUNCTION(BlueprintImplementableEvent, Category = "Projectile", meta = (DisplayName = "On Projectile Explode"))
	void K2_OnProjectileExplode();
};
