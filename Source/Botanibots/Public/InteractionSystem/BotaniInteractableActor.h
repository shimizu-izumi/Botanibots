// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BotaniInteractableActor.generated.h"

class UWidgetComponent;

UCLASS(Abstract, DisplayName = "Interactable Actor")
class BOTANIBOTS_API ABotaniInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	ABotaniInteractableActor();
};
