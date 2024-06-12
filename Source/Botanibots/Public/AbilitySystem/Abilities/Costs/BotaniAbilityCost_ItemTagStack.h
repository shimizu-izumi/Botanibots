// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BotaniAbilityCost.h"
#include "BotaniAbilityCost_ItemTagStack.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Item Tag Stack")
class BOTANIBOTS_API UBotaniAbilityCost_ItemTagStack : public UBotaniAbilityCost
{
	GENERATED_BODY()

public:
	UBotaniAbilityCost_ItemTagStack();
	
	//~ being UBotaniAbilityCost contract
	virtual bool CheckCost(const UBotaniAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UBotaniAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~ end UBotaniAbilityCost contract

protected:
	// How much of the tag to spend
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost")
	FScalableFloat Quantity;

	// Tag of the stack to be spent
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost")
	FGameplayTag StackTag;

	// Tag to send back as a response if this cost cannot be applied
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost")
	FGameplayTag FailureTag;
};
