// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Botanibots/Botanibots.h"
#include "Data/BotaniInputConfig.h"
#include "BotaniInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOTANIBOTS_API UBotaniInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UBotaniInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);
	
	template <class UserClass, typename StartedFuncType, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UBotaniInputConfig* InputConfig, UserClass* Object, StartedFuncType StartedFunc, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);
};

template <class UserClass, typename FuncType>
void UBotaniInputComponent::BindNativeAction(const UBotaniInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	if (!InputTag.IsValid())
	{
		if (bLogIfNotFound)
		{
			UE_LOG(LogBotaniInput, Error, TEXT("Bind Native Action was called with an invalid InputTag."));
		}
		return;
	}
	
	if (const UInputAction* InputAction = InputConfig->GetNativeInputActionByTag(InputTag))
	{
		BindAction(InputAction, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename StartedFuncType, typename PressedFuncType, typename ReleasedFuncType>
void UBotaniInputComponent::BindAbilityActions(const UBotaniInputConfig* InputConfig, UserClass* Object, StartedFuncType StartedFunc, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const FBotaniInputAction& InputAction : InputConfig->AbilityInputActions)
	{
		if (InputAction.InputAction && InputAction.AbilityTag.IsValid())
		{
			if (StartedFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Started, Object, StartedFunc, InputAction.AbilityTag);
			}
			
			if (PressedFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, InputAction.AbilityTag);
			}

			if (ReleasedFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.AbilityTag);
			}
		}
		else
		{
			UE_LOG(LogBotaniInput, Error, TEXT("BindAbilityActions was called with an invalid InputAction or AbilityTag."));
		}
	}
}
