// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "GameplayTagStackContainer.generated.h"

struct FGameplayTagStackContainer;
struct FGameplayTagStack;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStackValueChanged, const FGameplayTagStack&);

/*
 Gameplay Tag Stack Value
 Stores the value of a Gameplay Tag Stack
 */
USTRUCT(BlueprintType)
struct FGameplayTagStackValue
{
	GENERATED_BODY()

	FGameplayTagStackValue()
	{
	}

	FGameplayTagStackValue(int32 InMagnitude, bool InIsPersistent)
		: Magnitude(InMagnitude)
		, bIsPersistent(InIsPersistent)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = "Value")
	int32 Magnitude = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Value")
	bool bIsPersistent = false;

	//OPT: Eventuell noch mehr Magnitues hinzufügen wie zb bool, float etc ???
};

/*
 Gameplay Tag Stack
 Represents a stack of a single Gameplay Tag with a value
 */
USTRUCT(BlueprintType)
struct FGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FGameplayTagStack()
	{
	}

	FGameplayTagStack(const FGameplayTag& InGameplayTag, const FGameplayTagStackValue& InStackValue)
		: GameplayTag(InGameplayTag)
		, StackValue(InStackValue)
	{
	}

	//~Begin FGameplayTagStack Contract
	FGameplayTag GetStackTag() const { return GameplayTag; }
	FGameplayTagStackValue GetStackValue() const { return StackValue; }
	//~End FGameplayTagStack Contract

	bool operator==(const FGameplayTagStack& Other) const
	{
		return GameplayTag .MatchesTagExact(Other.GameplayTag);
	}

private:
	friend FGameplayTagStackContainer;

	UPROPERTY()
	FGameplayTag GameplayTag;

	UPROPERTY()
	FGameplayTagStackValue StackValue;
};

/*
 Gameplay Tag Stack Container
 Represents a container of Gameplay Tag Stacks
 */
USTRUCT(BlueprintType)
struct FGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()

public:

	FGameplayTagStackContainer()
	{
	}

	//~Begin FGameplayTagStackContainer Contract
	void AddStack(FGameplayTag GameplayTag, int32 StackCount, bool bPersistentStack);
	void RemoveStack(FGameplayTag GameplayTag, int32 StackCount);
	void EmptyStacks();
	int32 GetStackCount(const FGameplayTag& GameplayTag) const;
	bool ContainsStack(const FGameplayTag& GameplayTag) const;
	TArray<FGameplayTagStack> GetAllTagStacks() const;
	//~End FGameplayTagStackContainer Contract

	FOnStackValueChanged OnStackValueChangedDelegate;

	//~Begin FFastArraySerializer Interface
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End FFastArraySerializer Interface

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FGameplayTagStack, FGameplayTagStackContainer>(TagStacks, DeltaParams, *this);
	}

private:
	// Replicated list of tag stacks
	UPROPERTY()
	TArray<FGameplayTagStack> TagStacks;

	// Accelerated list of tag stacks for fast lookup
	TMap<FGameplayTag, FGameplayTagStackValue> TagToCountMap;
	
};

template<>
struct TStructOpsTypeTraits<FGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FGameplayTagStackContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};