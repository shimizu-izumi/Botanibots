// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BotaniInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FBotaniInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (categories = "Input.InputTag"))
	FGameplayTag AbilityTag;
};

/**
 *
 */
UCLASS()
class BOTANIBOTS_API UBotaniInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<FBotaniInputAction> AbilityInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<FBotaniInputAction> NativeInputActions;

	
	const UInputAction* GetAbilityInputActionByTag(const FGameplayTag& Tag, bool bLogNotFound = true) const;
	const UInputAction* GetNativeInputActionByTag(const FGameplayTag& Tag, bool bLogNotFound = true) const;
};
