// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Characters/BotaniEnemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BotaniAttributeSet.h"
#include "AbilitySystem/Component/BotaniAbilityComponent.h"


ABotaniEnemy::ABotaniEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UBotaniAbilityComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBotaniAttributeSet>(TEXT("AttributeSet"));
}

void ABotaniEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
