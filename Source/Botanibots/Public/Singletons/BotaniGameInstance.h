// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
//#include "GameInstance/CommonOnlineGameInstance.h"

#include "BotaniGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BOTANIBOTS_API UBotaniGameInstance :  public UGameInstance
{
	GENERATED_BODY()

public:
	FStreamableManager AssetLoader;
};
