// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Characters/BotaniPlayer.h"

#include "AbilitySystemComponent.h"
#include "PlayerStates/BotaniPlayerState.h"


ABotaniPlayer::ABotaniPlayer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABotaniPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize ability actor info
	InitAbilityActorInfo();
}

void ABotaniPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize ability actor info
	InitAbilityActorInfo();
}

void ABotaniPlayer::InitAbilityActorInfo()
{
	ABotaniPlayerState* BotaniPlayerState = GetPlayerState<ABotaniPlayerState>();
	if (!BotaniPlayerState)
	{
		return;
	}

	BotaniPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(BotaniPlayerState, this);

	AbilitySystemComponent = BotaniPlayerState->GetAbilitySystemComponent();
	AttributeSet = BotaniPlayerState->GetAttributeSet();

	AbilitySystemComponent->InitAbilityActorInfo(BotaniPlayerState, this);
}
