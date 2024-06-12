// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AbilitySystem/Abilities/BotaniAbility.h"

#include "AbilitySystem/Abilities/Costs/BotaniAbilityCost.h"
#include "Botanibots/Botanibots.h"


UBotaniAbility::UBotaniAbility(const FObjectInitializer& ObjectInitializer)
	: UGameplayAbility(ObjectInitializer)
{
}

AController* UBotaniAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* Controller = Cast<AController>(TestActor))
			{
				return Controller;
			}

			if (const APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

bool UBotaniAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	return true;
}

bool UBotaniAbility::CheckCost(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	// Check if we can afford the default cost
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	for (const auto& Cost : CustomCosts)
	{
		if (!Cost)
		{
			UE_LOG(LogBotaniAbilitySystem, Warning, TEXT("[%s]: Has an invalid cost."), *GetName());
			break;
		}
		
		if (!Cost->CheckCost(this, Handle, ActorInfo, OptionalRelevantTags))
		{
			return false;
		}
	}

	return true;
}

void UBotaniAbility::ApplyCost(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	if (!ActorInfo)
	{
		return;
	}

	for (const auto& Cost : CustomCosts)
	{
		if (!Cost)
		{
			UE_LOG(LogBotaniAbilitySystem, Warning, TEXT("[%s]: Has an invalid cost."), *GetName());
			break;
		}

		Cost->ApplyCost(this, Handle, ActorInfo, ActivationInfo);
	}

	ApplyAdditionalCost(Handle, *ActorInfo, ActivationInfo);
}

void UBotaniAbility::ApplyAdditionalCost_Implementation(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	return;
}
