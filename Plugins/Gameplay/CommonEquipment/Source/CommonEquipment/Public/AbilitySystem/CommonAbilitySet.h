// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "GameplayAbilitySpecHandle.h"
#include "CommonAbilitySet.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

/*
 Ability Set Gameplay Ability
 A single Gameplay Ability in an Ability Set
 */
USTRUCT(BlueprintType)
struct FCommonAbilitySet_GameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Input.InputTag"))
	FGameplayTag InputTag;
};

/*
 Ability Set Gameplay Effect
 A single Gameplay Effect in an Ability Set
 */
USTRUCT(BlueprintType)
struct FCommonAbilitySet_GameplayEffect
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.f;
};

/*
 Ability Set Attribute Set
 A single Attribute Set in an Ability Set
 */
USTRUCT(BlueprintType)
struct FCommonAbilitySet_AttributeSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet = nullptr;
};

/*
 Ability Set Granted Handles
 Stores Handles to what has been granted by the Ability Set
 */
USTRUCT(BlueprintType)
struct FCommonAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* AttributeSet);

	void TakeFromAbilitySystem(UAbilitySystemComponent* TargetASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

/*
 Ability Set 
 */
UCLASS(BlueprintType, Const)
class COMMONEQUIPMENT_API UCommonAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCommonAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UAbilitySystemComponent* TargetASC, FCommonAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const;
	void TakeFromAbilitySystem(UAbilitySystemComponent* TargetASC, FCommonAbilitySet_GrantedHandles* Handles) const;

protected:
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FCommonAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FCommonAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta=(TitleProperty=AttributeSet))
	TArray<FCommonAbilitySet_AttributeSet> GrantedAttributes;
};
