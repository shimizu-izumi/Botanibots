// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagStacks/GameplayTagStackContainer.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"
#include "TagStackComponent.generated.h"


UCLASS(ClassGroup=("Inventory"), meta=(BlueprintSpawnableComponent, DisplayName = "Tag Stack Component"), DisplayName = "Tag Stack Component")
class COMMONEQUIPMENT_API UTagStackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTagStackComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin UTagStackComponent Contract
	FGameplayTagStackContainer& GetStackContainer();
	TArray<FGameplayTagStack> GetAllStacks() const;
	virtual void AddStatTagStack(const FGameplayTag& GameplayTag, int32 StackCount, bool bPersistentStat);
	virtual void StorePersistentStatTagStacks(const FGameplayTagStackContainer& StackContainer);
	virtual void RemoveStatTagStack(const FGameplayTag& GameplayTag, int32 StackCount);
	virtual void ClearStatTagStacks();
	virtual int32 GetTagStackCount(const FGameplayTag& GameplayTag) const;
	virtual bool HasTagStack(const FGameplayTag& GameplayTag) const;
	virtual bool HasAnyStacks() const;
	//~End UTagStackComponent Contract
	

#pragma region BlueprintFunctions

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagStackComponent", meta = (DisplayName = "Get All Stacks"))
	TArray<FGameplayTagStack> K2_GetAllStacks() const { return GetAllStacks(); }

	UFUNCTION(BlueprintCallable, Category = "TagStackComponent", meta = (DisplayName = "Add Stat Tag Stack"))
	void K2_AddStatTagStack(const FGameplayTag& GameplayTag, int32 StackCount, bool bPersistentStat) { AddStatTagStack(GameplayTag, StackCount, bPersistentStat); }

	UFUNCTION(BlueprintCallable, Category = "TagStackComponent", meta = (DisplayName = "Store Persistent Stat Tag Stacks"))
	void K2_StorePersistentStatTagStacks(const FGameplayTagStackContainer& StackContainer) { StorePersistentStatTagStacks(StackContainer); }

	UFUNCTION(BlueprintCallable, Category = "TagStackComponent", meta = (DisplayName = "Remove Stat Tag Stack"))
	void K2_RemoveStatTagStack(const FGameplayTag& GameplayTag, int32 StackCount) { RemoveStatTagStack(GameplayTag, StackCount); }

	UFUNCTION(BlueprintCallable, Category = "TagStackComponent", meta = (DisplayName = "Clear Stat Tag Stacks"))
	void K2_ClearStatTagStacks() { ClearStatTagStacks(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagStackComponent", meta = (DisplayName = "Get Tag Stack Count"))
	int32 K2_GetTagStackCount(const FGameplayTag& GameplayTag) const { return GetTagStackCount(GameplayTag); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagStackComponent", meta = (DisplayName = "Has Tag Stack"))
	bool K2_HasTagStack(const FGameplayTag& GameplayTag) const { return HasTagStack(GameplayTag); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagStackComponent", meta = (DisplayName = "Has Any Stacks"))
	bool K2_HasAnyStacks() const { return HasAnyStacks(); }

#pragma endregion

private:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer TagStacks;
};
