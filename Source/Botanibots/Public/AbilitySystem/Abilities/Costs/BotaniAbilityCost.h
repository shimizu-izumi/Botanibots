// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayAbilitySpec.h"
#include "UObject/Object.h"
#include "BotaniAbilityCost.generated.h"

struct FGameplayTagContainer;
struct FGameplayAbilityActorInfo;
struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;
class UBotaniAbility;
class UObject;

/**
Ability Cost
Base class for additional costs used for ammo, etc.
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class BOTANIBOTS_API UBotaniAbilityCost : public UObject
{
	GENERATED_BODY()

public:
	UBotaniAbilityCost()
	{
	}

	/*
	Check Cost
	Checks if we can afford this cost.
	 */
	virtual bool CheckCost(const UBotaniAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	/*
	Apply Cost
	Applies the ability's cost to the target
	 */
	virtual void ApplyCost(const UBotaniAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}

	//OPT: Eventuell sollte die cost erst gemacht werden bei einem hit?
	//OPT: Für halt besondere waffen oder so
};
