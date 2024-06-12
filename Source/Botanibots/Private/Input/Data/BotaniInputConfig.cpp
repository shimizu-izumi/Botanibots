// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Input/Data/BotaniInputConfig.h"

#include "Botanibots/Botanibots.h"

const UInputAction* UBotaniInputConfig::GetAbilityInputActionByTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FBotaniInputAction& AbilityInput : AbilityInputActions)
	{
		if (AbilityInput.AbilityTag.MatchesTagExact(Tag) && AbilityInput.InputAction)
		{
			return AbilityInput.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogBotaniInput, Error, TEXT("No Ability Input Action found for Tag: [%s] in: [%s"), *Tag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}

const UInputAction* UBotaniInputConfig::GetNativeInputActionByTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FBotaniInputAction& AbilityInput : NativeInputActions)
	{
		if (AbilityInput.AbilityTag.MatchesTagExact(Tag) && AbilityInput.InputAction)
		{
			return AbilityInput.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogBotaniInput, Error, TEXT("No Native Input Action found for Tag: [%s] in: [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
