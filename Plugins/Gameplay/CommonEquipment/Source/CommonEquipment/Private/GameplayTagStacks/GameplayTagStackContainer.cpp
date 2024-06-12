// Copyright © 2024 Botanibots Team. All rights reserved.


#include "GameplayTagStacks/GameplayTagStackContainer.h"

void FGameplayTagStackContainer::AddStack(FGameplayTag GameplayTag, int32 StackCount, bool bPersistentStack)
{
	// Check that the GameplayTag is valid
	if (!GameplayTag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid GameplayTag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}

	// INDEX_NONE means that the stack is infinite
	if (StackCount == INDEX_NONE)
	{
		StackCount = INT_MAX;
	}

	// Check that at least one stack is being added
	if (StackCount <= 0)
	{
		FFrame::KismetExecutionMessage(TEXT("At least one stack must be added to the GameplayTagStackContainer"), ELogVerbosity::Warning);
		return;
	}

	for (FGameplayTagStack& Stack : TagStacks)
	{
		// If the tag already exists in the stack, add the stack count to the existing stack
		if (Stack.GameplayTag.MatchesTagExact(GameplayTag))
		{
			const int32 NewCount = Stack.StackValue.Magnitude + StackCount;
			Stack.StackValue.Magnitude = NewCount;
			TagToCountMap[GameplayTag].Magnitude = NewCount;
			MarkItemDirty(Stack);
			OnStackValueChangedDelegate.Broadcast(Stack);
			return;
		}
	}

	// If the tag does not exist in the stack, create a new stack
	FGameplayTagStack& NewStack = TagStacks.Emplace_GetRef(GameplayTag,FGameplayTagStackValue(StackCount, bPersistentStack));
	TagToCountMap.Add(GameplayTag, FGameplayTagStackValue(StackCount, bPersistentStack));
	OnStackValueChangedDelegate.Broadcast(NewStack);
	MarkItemDirty(NewStack);
}

void FGameplayTagStackContainer::RemoveStack(FGameplayTag GameplayTag, int32 StackCount)
{
	// Check that the GameplayTag is valid
	if (!GameplayTag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid GameplayTag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}

	// Check that at least one stack is being removed
	if (StackCount <= 0)
	{
		FFrame::KismetExecutionMessage(TEXT("StackCount in RemoveStack must be at least 1"), ELogVerbosity::Warning);
		return;
	}

	for (auto It = TagStacks.CreateIterator(); It; ++It)
	{
		FGameplayTagStack& Stack = *It;

		if (Stack.GameplayTag.MatchesTagExact(GameplayTag))
		{
			// If the stack count to remove is greater or equal to the current stack count, remove the stack
			if (Stack.StackValue.Magnitude <= StackCount)
			{
				//OPT: mach das hier mal besser
				OnStackValueChangedDelegate.Broadcast(Stack);
				It.RemoveCurrent();
				TagToCountMap.Remove(GameplayTag);
				MarkArrayDirty();
			}
			// If the stack count to remove is less than the current stack count, subtract the stack count
			else
			{
				const int32 NewCount = Stack.StackValue.Magnitude - StackCount;
				Stack.StackValue.Magnitude = NewCount;
				TagToCountMap[GameplayTag].Magnitude = NewCount;
				OnStackValueChangedDelegate.Broadcast(Stack);
				MarkItemDirty(Stack);
			}
			return;
		}
	}
}

void FGameplayTagStackContainer::EmptyStacks()
{
	TagStacks.Empty();
	TagToCountMap.Empty();
	MarkArrayDirty();
}

int32 FGameplayTagStackContainer::GetStackCount(const FGameplayTag& GameplayTag) const
{
	return TagToCountMap.FindRef(GameplayTag).Magnitude;
}

bool FGameplayTagStackContainer::ContainsStack(const FGameplayTag& GameplayTag) const
{
	return TagToCountMap.Contains(GameplayTag);
}

TArray<FGameplayTagStack> FGameplayTagStackContainer::GetAllTagStacks() const
{
	return TagStacks;
}

void FGameplayTagStackContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const int32 Index : RemovedIndices)
	{
		const FGameplayTag Tag = TagStacks[Index].GameplayTag;
		TagToCountMap.Remove(Tag);
		OnStackValueChangedDelegate.Broadcast(FGameplayTagStack(Tag, FGameplayTagStackValue(0, false)));
	}
}

void FGameplayTagStackContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 Index : AddedIndices)
	{
		const FGameplayTagStack& Stack = TagStacks[Index];
		TagToCountMap.Add(Stack.GameplayTag, FGameplayTagStackValue(Stack.StackValue.Magnitude, Stack.StackValue.bIsPersistent));
		OnStackValueChangedDelegate.Broadcast(FGameplayTagStack(Stack.GameplayTag, FGameplayTagStackValue(Stack.StackValue.Magnitude, Stack.StackValue.bIsPersistent)));
	}
}

void FGameplayTagStackContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const int32 Index : ChangedIndices)
	{
		const FGameplayTagStack& Stack = TagStacks[Index];
		TagToCountMap[Stack.GameplayTag].Magnitude = Stack.StackValue.Magnitude;
		OnStackValueChangedDelegate.Broadcast(FGameplayTagStack(Stack.GameplayTag, FGameplayTagStackValue(Stack.StackValue.Magnitude, Stack.StackValue.bIsPersistent)));
	}
}
