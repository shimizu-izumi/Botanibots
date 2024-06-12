// Copyright © 2024 Botanibots Team. All rights reserved.


#include "EquipmentSystem/Weapons/Projectiles/BotaniProjectileBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ABotaniProjectileBase::ABotaniProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bNetLoadOnClient = true;
	AActor::SetReplicateMovement(true);

	ProjectileHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollisionComponent"));
	if (ProjectileHitbox)
	{
		SetRootComponent(ProjectileHitbox);
	}

	ProjectileEnvironmentHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileEnvironmentCollisionComponent"));
	if (ProjectileEnvironmentHitbox)
	{
		ProjectileEnvironmentHitbox->SetupAttachment(ProjectileHitbox);
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->SetUpdatedComponent(ProjectileHitbox);
	}
	
	InitialLifeSpan = MaxProjectileLifeTime;
	AActor::SetLifeSpan(MaxProjectileLifeTime);
}

void ABotaniProjectileBase::ExplodeProjectile()
{
	// Call the Blueprint Event
	K2_OnProjectileExplode();

	// Deactivate the Projectile Movement
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->Deactivate();
	}
	
	if (!HasAuthority())
	{
		return;
	}

	TArray<TObjectPtr<AActor>> HitActors;
	TArray<TObjectPtr<AActor>> IgnoredActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	const bool bHitSomething = UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), ExplodeRadius, ObjectTypes, nullptr, IgnoredActors, HitActors);

	// Only continue if we hit something
	if (bHitSomething && HitActors.Num() > 0)
	{
		
	}
}

void ABotaniProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	AActor::SetLifeSpan(MaxProjectileLifeTime);

	if (ProjectileRibbonSystem)
	{
		ProjectileRibbonComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		ProjectileRibbonSystem,
		ProjectileHitbox,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true);	
	}
}
