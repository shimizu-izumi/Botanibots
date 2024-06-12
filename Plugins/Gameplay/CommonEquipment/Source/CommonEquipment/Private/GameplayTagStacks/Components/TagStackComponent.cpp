// Copyright © 2024 Botanibots Team. All rights reserved.


#include "GameplayTagStacks/Components/TagStackComponent.h"
#include "GameplayTagStacks/GameplayTagStackContainer.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TagStackComponent)

UTagStackComponent::UTagStackComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

FGameplayTagStackContainer& UTagStackComponent::GetStackContainer()
{
	return TagStacks;
}

TArray<FGameplayTagStack> UTagStackComponent::GetAllStacks() const
{
	return TagStacks.GetAllTagStacks();
}

void UTagStackComponent::AddStatTagStack(const FGameplayTag& GameplayTag, int32 StackCount, bool bPersistentStat)
{
	TagStacks.AddStack(GameplayTag, StackCount, bPersistentStat);
}

void UTagStackComponent::StorePersistentStatTagStacks(const FGameplayTagStackContainer& StackContainer)
{
	for (const auto& Stack : StackContainer.GetAllTagStacks())
	{
		if (Stack.GetStackValue().bIsPersistent)
		{
			AddStatTagStack(Stack.GetStackTag(), Stack.GetStackValue().Magnitude, Stack.GetStackValue().bIsPersistent);
		}
	}
}

void UTagStackComponent::RemoveStatTagStack(const FGameplayTag& GameplayTag, int32 StackCount)
{
	TagStacks.RemoveStack(GameplayTag, StackCount);
}

void UTagStackComponent::ClearStatTagStacks()
{
	TagStacks.EmptyStacks();
}

int32 UTagStackComponent::GetTagStackCount(const FGameplayTag& GameplayTag) const
{
	return TagStacks.GetStackCount(GameplayTag);
}

bool UTagStackComponent::HasTagStack(const FGameplayTag& GameplayTag) const
{
	return TagStacks.ContainsStack(GameplayTag);
}

bool UTagStackComponent::HasAnyStacks() const
{
	return TagStacks.GetAllTagStacks().Num() > 0;
}


void UTagStackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTagStackComponent, TagStacks);
}
