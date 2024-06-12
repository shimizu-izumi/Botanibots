// Copyright © 2024 Botanibots Team. All rights reserved.


#include "GameObjects/Interactables/Launcher/BotaniLauncher.h"

#include "Components/SphereComponent.h"
#include "GameplayCueFunctionLibrary.h"
#include "GameFramework/Character.h"


ABotaniLauncher::ABotaniLauncher()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
	if (SphereCollisionComponent)
	{
		SphereCollisionComponent->InitSphereRadius(100.f);
		SphereCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

#if WITH_EDITOR
void ABotaniLauncher::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	// Update TargetBounds Location so that it keeps its world location
	if (bUseTarget && bLockTargetBounds)
	{
		TargetBounds = TargetBounds - GetActorLocation();
	}
}
#endif

void ABotaniLauncher::BeginPlay()
{
	Super::BeginPlay();
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABotaniLauncher::OnOverlapBegin);
}

void ABotaniLauncher::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappedCharacter = Cast<ACharacter>(OtherActor);
	if (!OverlappedCharacter || !HasAuthority())
	{
		return;
	}

	if (TargetOverlapped(OverlappedCharacter))
	{
		NetMulticast_ActivationVFX();		
	}
}

void ABotaniLauncher::NetMulticast_ActivationVFX_Implementation()
{
	FGameplayCueParameters Parameters;
	Parameters.Location = GetActorLocation();
	Parameters.EffectCauser = this;
	Parameters.Normal = FVector::UpVector;

	UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(OverlappedCharacter, LauncherActivateCueTag, Parameters);
}
