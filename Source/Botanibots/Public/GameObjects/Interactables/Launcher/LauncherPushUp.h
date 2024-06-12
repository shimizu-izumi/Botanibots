// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BotaniLauncher.h"
#include "LauncherPushUp.generated.h"

UCLASS()
class BOTANIBOTS_API ALauncherPushUp : public ABotaniLauncher
{
	GENERATED_BODY()

public:
	ALauncherPushUp();
	
protected:
	virtual bool TargetOverlapped(ACharacter* InOverlappedCharacter) override;
};
