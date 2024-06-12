// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BotaniCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBotaniCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BOTANIBOTS_API IBotaniCombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetFirstPersonMesh() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetThirdPersonMesh() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() const;
};
