// Copyright © 2024 Botanibots Team. All rights reserved.


#include "PlayerStates/BotaniPlayerState.h"

#include "AbilitySystem/BotaniAttributeSet.h"
#include "AbilitySystem/Component/BotaniAbilityComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ABotaniPlayerState::ABotaniPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBotaniAbilityComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBotaniAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ABotaniPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
