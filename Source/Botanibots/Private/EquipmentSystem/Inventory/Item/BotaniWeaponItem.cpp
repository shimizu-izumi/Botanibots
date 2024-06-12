// Copyright © 2024 Botanibots Team. All rights reserved.


#include "EquipmentSystem/Inventory/Item/BotaniWeaponItem.h"
#include "NiagaraFunctionLibrary.h"

ABotaniWeaponItem::ABotaniWeaponItem()
{
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	if (WeaponMeshComponent)
	{
		SetRootComponent(WeaponMeshComponent);
		WeaponMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		WeaponMeshComponent->SetGenerateOverlapEvents(false);
	}
}

void ABotaniWeaponItem::FireWeapon(const FBotaniWeaponFirePayload& FirePayload)
{
	// Spawn Impact Effects
	if (FirePayload.bIsHitScan && FirePayload.IsValid())
	{
		for (const FHitResult& HitResult : FirePayload.HitResults)
		{
			SpawnImpactEffect(HitResult);
		}
	}
}

void ABotaniWeaponItem::FireWeaponOutOfAmmo()
{
}

void ABotaniWeaponItem::SpawnImpactEffect(const FHitResult& HitResult)
{
	// If we have no impact effects, we can't spawn anything
	if (!ImpactEffects.Num())
	{
		return;
	}
	
	// If the hit actor is a pawn, we should attach the impact effect to it if the weapon is set up to do so
	const bool bShouldAttach = bImpactEffectAttachedToHitActor && HitResult.GetActor() ?  HitResult.GetActor()->IsA(APawn::StaticClass()) : false;

	// If we have a specific impact effect for the surface type, use that, otherwise use the default
	UNiagaraSystem* ImpactEffect;
	if (ImpactEffects.Contains(HitResult.PhysMaterial->SurfaceType))
	{
		ImpactEffect = ImpactEffects[HitResult.PhysMaterial->SurfaceType];
	}
	else
	{
		ImpactEffect = ImpactEffects[SurfaceType_Default];
	}

	// Spawn the impact effect, either attached to the hit actor or at the impact point
	if (bShouldAttach)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			ImpactEffect,
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			HitResult.ImpactNormal.Rotation(),
			EAttachLocation::SnapToTarget,
			true);
	}
	else
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ImpactEffect,
			HitResult.ImpactPoint,
			HitResult.ImpactNormal.Rotation());
	}
}

void ABotaniWeaponItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABotaniWeaponItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
