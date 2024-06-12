// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AbilitySystem/Component/BotaniAbilityComponent.h"

#include "AbilitySystem/Abilities/BotaniAbility.h"
#include "Botanibots/Botanibots.h"


//OPT: Evtl entpuppt sich das später als umständlicher. Aktuell ist es aber echt gut
void UBotaniAbilityComponent::AbilityInputStarted(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		UE_LOG(LogBotaniAbilitySystem, Error, TEXT("Tag [%s] is invalid"), *InputTag.ToString());
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			const UBotaniAbility* BotaniAbility = Cast<UBotaniAbility>(AbilitySpec.Ability);

			if (!BotaniAbility)
			{
				break;
			}

			if (BotaniAbility->GetActivationPolicy() == EBotaniAbilityActivationPolicy::OnInputTriggered)
			{
				// Store them as Pressed
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UBotaniAbilityComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		UE_LOG(LogBotaniAbilitySystem, Error, TEXT("Tag [%s] is invalid"), *InputTag.ToString());
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{

			const UBotaniAbility* BotaniAbility = Cast<UBotaniAbility>(AbilitySpec.Ability);

			if (!BotaniAbility)
			{
				break;
			}
			if (BotaniAbility->GetActivationPolicy() == EBotaniAbilityActivationPolicy::WhileInputActive)
			{
				// Store them as Held
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UBotaniAbilityComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		UE_LOG(LogBotaniAbilitySystem, Error, TEXT("Tag [%s] is invalid"), *InputTag.ToString());
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
		{
			// Store them as Released and remove from Held
			InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
			InputHeldSpecHandles.Remove(AbilitySpec.Handle);
		}
	}
}

void UBotaniAbilityComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{

	// List of Abilities that will be activated
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	// Check for Abilities that were held and store them as to be activated
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			const UBotaniAbility* BotaniAbility = Cast<UBotaniAbility>(AbilitySpec->Ability);

			if (!BotaniAbility)
			{
				break;
			}

			if (BotaniAbility->GetActivationPolicy() == EBotaniAbilityActivationPolicy::WhileInputActive)
			{
				AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
			}
		}
	}

	// Check for Abilities that were pressed and store them as to be activated
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			const UBotaniAbility* BotaniAbility = Cast<UBotaniAbility>(AbilitySpec->Ability);

			if (!BotaniAbility)
			{
				break;
			}

			AbilitySpecInputPressed(*AbilitySpec);
			AbilitySpec->InputPressed = true;

			if (!AbilitySpec->IsActive())
			{
				if (BotaniAbility->GetActivationPolicy() == EBotaniAbilityActivationPolicy::OnInputTriggered)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	// Now Try to Activate the Abilities
	for (const FGameplayAbilitySpecHandle& SpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(SpecHandle);
	}

	// Check for Abilities that were released
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (!AbilitySpec->Ability)
			{
				return;
			}
			
			AbilitySpecInputReleased(*AbilitySpec);
			AbilitySpec->InputPressed = false;
		}
	}

	// Clear the cached spec handles
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UBotaniAbilityComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UBotaniAbilityComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	/* Von unreal engine selber
	Man soll ja den input nicht direkt replicaten da es sonst das netzwerk total überlasten würde.
	Deswegen machen wir hier ganz brav nur ein InvokeReplicatedEvent damit OnInputPressed aufgerufen wird.
	*/
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UBotaniAbilityComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	/* Von unreal engine selber
	Man soll ja den input nicht direkt replicaten da es sonst das netzwerk total überlasten würde.
	Deswegen machen wir hier ganz brav nur ein InvokeReplicatedEvent damit OnInputReleased aufgerufen wird.
	*/
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}
