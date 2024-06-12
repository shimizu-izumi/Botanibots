// Copyright © 2024 Botanibots Team. All rights reserved.


#include "InteractionSystem/Component/BotaniInteractionComponent.h"

#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "Pickup/Interface/CommonInteractionInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BotaniInteractionComponent)

UBotaniInteractionComponent::UBotaniInteractionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UBotaniInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PotentialInteractable);
}

void UBotaniInteractionComponent::SetPotentialInteractable(AActor* NewInteractable)
{
	// Check that we found a new interactable
	if (NewInteractable == PotentialInteractable)
	{
		return;
	}

	// Check that we have a valid owner
	if (!OwningPawn)
	{
		ClearInteractionTimer();
		return;
	}

	// Store the old Interactable
	AActor* OldInteractable = PotentialInteractable;

	// Update the new Interactable
	PotentialInteractable = NewInteractable;

	
	if (ICommonInteractionInterface* OldInteractableInterface = Cast<ICommonInteractionInterface>(OldInteractable))
	{
		OldInteractableInterface->LeaveInteractable(OwningPawn);
	}
	if (ICommonInteractionInterface* NewInteractableInterface = Cast<ICommonInteractionInterface>(NewInteractable))
	{
		NewInteractableInterface->TouchInteractable(OwningPawn);
	}
}

void UBotaniInteractionComponent::InteractionTimer()
{
	ScanForInteractables();
}

void UBotaniInteractionComponent::ScanForInteractables()
{
	if (!OwningPawn)
	{
		ClearInteractionTimer();
		return;
	}

	// Perform a trace to find interactables
	FHitResult HitResult;
	PerformInteractionTrace(HitResult);

	// Check if we hit something
	if (!HitResult.bBlockingHit || !HitResult.GetActor())
	{
		if (PotentialInteractable != nullptr)
		{
			SetPotentialInteractable(nullptr);
		}
		return;
	}

	// If we hit an actor, and its interactable, set it as the potential interactable
	if (HitResult.GetActor()->Implements<UCommonInteractionInterface>())
	{
		SetPotentialInteractable(HitResult.GetActor());
	}
	else
	{
		SetPotentialInteractable(nullptr);
	}
}

bool UBotaniInteractionComponent::TryCommitInteraction()
{
	if (OwningPawn ? !OwningPawn->HasAuthority() : true)
	{
		return false;
	}

	FHitResult HitResult;
	PerformInteractionTrace(HitResult);

	// The Server should now try to find the interactable
	if (HitResult.bBlockingHit && HitResult.GetActor())
	{
		PotentialInteractable = HitResult.GetActor();
	}
	else
	{
		return false;
	}

	// Check if we can interact with the interactable
	if (const ICommonInteractionInterface* InteractablePtr = Cast<ICommonInteractionInterface>(PotentialInteractable))
	{
		if (!InteractablePtr->CanInteract(OwningPawn))
		{
			return false;
		}

		return CommitInteraction(PotentialInteractable);
	}

	// If we arrived here, we failed to interact with the interactable
	return false;
}

void UBotaniInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Start the Interaction Timer on the local player only -> Performance Optimization
	// NOTE: The Server will still authorize the interaction
	OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn->IsLocallyControlled())
	{
		InteractionTimerDelegate.BindUObject(this, &ThisClass::InteractionTimer);
		GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, InteractionTimerDelegate, 0.05f, true);
	}
}

bool UBotaniInteractionComponent::CommitInteraction(AActor* Interactable)
{
	if (!OwningPawn->HasAuthority() || !IsValid(Interactable))
	{
		return false;
	}
	
	if (ICommonInteractionInterface* InteractablePtr = Cast<ICommonInteractionInterface>(Interactable))
	{
		if (InteractablePtr->CanInteract(OwningPawn))
		{
			return InteractablePtr->CommitInteraction(OwningPawn);	
		}
	}
	return false;
}

void UBotaniInteractionComponent::PerformInteractionTrace(FHitResult& OutHitResult) const
{
	if (!OwningPawn)
	{
		return;
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwningPawn);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse = ECR_Block;

	FVector StartingLocation = OwningPawn->GetActorLocation();

	if (USceneComponent* CamComponent = OwningPawn->GetComponentByClass<UCameraComponent>())
	{
		StartingLocation = CamComponent->GetComponentLocation();
	}

	const FVector EndLocation = StartingLocation + OwningPawn->GetControlRotation().Vector() * 250.f;

	GetWorld()->LineTraceSingleByChannel(OutHitResult, StartingLocation, EndLocation, ECC_Visibility, QueryParams, ResponseParams);
}

void UBotaniInteractionComponent::ClearInteractionTimer()
{
	InteractionTimerDelegate.Unbind();
	GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
	InteractionTimerHandle.Invalidate();
}

void UBotaniInteractionComponent::OnRep_PotentialInteractable()
{
}
