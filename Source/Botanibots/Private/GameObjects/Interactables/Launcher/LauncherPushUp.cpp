// Copyright © 2024 Botanibots Team. All rights reserved.


#include "GameObjects/Interactables/Launcher/LauncherPushUp.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


ALauncherPushUp::ALauncherPushUp()
{
}

bool ALauncherPushUp::TargetOverlapped(ACharacter* InOverlappedCharacter)
{
	const bool bIsVelocityBelowThreshold = InOverlappedCharacter->GetCharacterMovement()->GetLastUpdateVelocity().Z < MinimumVerticalVelocityAllowed;
	if (bIsVelocityBelowThreshold && bDoNotTriggerOnFallEntry)
	{
		return false;
	}

	// Push the character
	FVector LaunchDirection;
	if (bUseTarget)
	{
		// Calculate the direction to launch the character
		const FVector CalculatedTargetBounds = InOverlappedCharacter->GetActorForwardVector() * UKismetMathLibrary::VSizeXY(TargetBounds) * 2.1f;;
		LaunchDirection = UKismetMathLibrary::ClampVectorSize(FVector(
			CalculatedTargetBounds.X,
			CalculatedTargetBounds.Y,
			FMath::Clamp(TargetBounds.Z * 2.f, 250.f, 850.f)
			), -12000.f, 12000.f);
	}
	else
	{
		LaunchDirection = FVector(InOverlappedCharacter->GetActorForwardVector().X, InOverlappedCharacter->GetActorForwardVector().Y, UpOverride);
	}

	InOverlappedCharacter->LaunchCharacter(LaunchDirection, false, false);
	return true;
}

