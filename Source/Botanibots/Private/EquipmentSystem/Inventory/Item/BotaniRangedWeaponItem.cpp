// Copyright © 2024 Botanibots Team. All rights reserved.


#include "EquipmentSystem/Inventory/Item/BotaniRangedWeaponItem.h"
#include "NiagaraFunctionLibrary.h"
#include "Interfaces/BotaniCombatInterface.h"

ABotaniRangedWeaponItem::ABotaniRangedWeaponItem()
{
}

void ABotaniRangedWeaponItem::FireWeapon(const FBotaniWeaponFirePayload& FirePayload)
{
	Super::FireWeapon(FirePayload);

	// Muzzle should be attached
	if (MuzzleFlashEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
		MuzzleFlashEffect,
		WeaponMeshComponent,
		MuzzleSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true
		);
	}

	// Muzzle Falloff should not be attached
	if (MuzzleFalloffEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		MuzzleFalloffEffect,
		WeaponMeshComponent->GetSocketLocation(MuzzleFalloffSocketName),
		WeaponMeshComponent->GetSocketRotation(MuzzleFalloffSocketName)
		);	
	}
}

USkeletalMeshComponent* ABotaniRangedWeaponItem::GetWeaponMesh_Implementation() const
{
	return WeaponMeshComponent;
}

void ABotaniRangedWeaponItem::BeginPlay()
{
	Super::BeginPlay();
}
