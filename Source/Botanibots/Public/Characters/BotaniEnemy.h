// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BotaniCharacter.h"
#include "BotaniEnemy.generated.h"

UCLASS()
class BOTANIBOTS_API ABotaniEnemy : public ABotaniCharacter
{
	GENERATED_BODY()

public:
	ABotaniEnemy();

protected:
	virtual void BeginPlay() override;
};
