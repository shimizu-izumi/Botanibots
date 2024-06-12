// Copyright © 2024 Botanibots Team. All rights reserved.


#include "InteractionSystem/BotaniInteractableActor.h"

#include "Components/WidgetComponent.h"

ABotaniInteractableActor::ABotaniInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
}

