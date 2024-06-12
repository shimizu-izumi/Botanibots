// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BotaniCharacter.h"
#include "BotaniPlayer.generated.h"

UCLASS()
class BOTANIBOTS_API ABotaniPlayer : public ABotaniCharacter
{
	GENERATED_BODY()

public:
	ABotaniPlayer();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
private:
	virtual void InitAbilityActorInfo() override;
};
