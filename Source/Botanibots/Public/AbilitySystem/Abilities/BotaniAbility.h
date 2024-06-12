// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BotaniAbility.generated.h"


/*
Ability Activation Policy
Defines how an ability will be activated
 */
UENUM(BlueprintType)
enum class EBotaniAbilityActivationPolicy : uint8
{
	/*
	On Input Triggered
	Try to activate the ability when the input is triggered
	 */
	OnInputTriggered,

	/*
	While Input Active
	Continually try to activate the ability while the input is active/held
	 */
	WhileInputActive,

	/*
	On Spawn
	TODO: AKTUELL NOCH NICHT IMPLEMENTIERT
	Try to activate the ability when an avatar is assigned
	Kinda like a passive ability
	 */
	OnSpawn
};


/**
Botani Ability
The base gameplay ability class used by this project.
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class BOTANIBOTS_API UBotaniAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBotaniAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Botani|Ability")
	AController* GetControllerFromActorInfo() const;

	EBotaniAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:
	//~ Begin UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	//~ End UGameplayAbility interface

	//~ Begin UBotaniAbility Contract
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void ApplyAdditionalCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;

	/*
	Additional Costs
	List of Additional Costs that will be applied when the ability is activated
	 */
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Costs")
	TArray<class UBotaniAbilityCost*> CustomCosts;

	/*
	Activation Policy
	Defines how the ability will be activated
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Activation")
	EBotaniAbilityActivationPolicy ActivationPolicy = EBotaniAbilityActivationPolicy::OnInputTriggered;
};
