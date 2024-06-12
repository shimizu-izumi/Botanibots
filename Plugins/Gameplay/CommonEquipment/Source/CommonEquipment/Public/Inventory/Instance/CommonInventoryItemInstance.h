// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagStacks/GameplayTagStackContainer.h"
#include "Inventory/Definition/CommonItemDefinition.h"
#include "UObject/Object.h"
#include "CommonInventoryItemInstance.generated.h"

class UCommonItemDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStackValueChangedSignature, const FGameplayTagStack&, Stack);

/*
 Inventory Item Instance
 An Instance of an Inventory Item
 */
UCLASS(BlueprintType, Blueprintable)
class COMMONEQUIPMENT_API UCommonInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UCommonInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin UCommonInventoryItemInstance Contract
	virtual UCommonItemDefinition* GetItemDef() const;
	virtual void SetItemDef(UCommonItemDefinition* ItemDef);
	virtual void SetOwnerComponent(UActorComponent* Owner);
	virtual int32 GetItemStackCount();
	virtual FGameplayTagStackContainer& GetStackContainer() { return TagStacks; }
	virtual void OnInstanceCreated(UCommonItemDefinition* ItemDef, UActorComponent* OwningComponent, FGameplayTagContainer CreationContext);
	virtual void OnEquipped();
	virtual void OnUnequipped();
	//~End UCommonInventoryItemInstance Contract

	//~Begin FGameplayTagStacks Contract
	void AddStatTagStack(FGameplayTag StatTag, int32 StackCount, bool bPersistentStat);
	void RemoveStatTagStack(FGameplayTag StatTag, int32 StackCount);
	int32 GetStatTagStackCount(FGameplayTag StatTag) const;
	bool HasTagStack(FGameplayTag StatTag) const;
	//~End FGameplayTagStacks Contract

	//~Begin UObject Interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End UObject Interface

#pragma region BlueprintFunctions
	/*
	 *
	 */
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Item", meta = (DisplayName = "On Stack Value Changed"))
	FOnStackValueChangedSignature OnStackValueChangedDelegate;
	
	/*
	 Get Item Definition
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Item", meta = (DisplayName = "Get Item Definition"))
	UPARAM(DisplayName = "Item Definition") UCommonItemDefinition* K2_GetItemDef() const { return GetItemDef(); }
	
	/*
	 Get Item Stack Count
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Item", meta = (DisplayName = "Get Item Stack Count"))
	UPARAM(DisplayName = "Stack Count") int32 K2_GetItemStackCount()
	{
		return GetItemStackCount();
	}
	
	/*
	 Add Stat Tag Stack
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory|Item|TagStacks", meta = (DisplayName = "Add Stat Tag Stack"))
	void K2_AddStatTagStack(FGameplayTag StatTag, int32 StackCount, bool bPersistentStat)
	{
		AddStatTagStack(StatTag, StackCount, bPersistentStat);
	}

	/*
	 Remove Stat Tag Stack
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory|Item|TagStacks", meta = (DisplayName = "Remove Stat Tag Stack"))
	void K2_RemoveStatTagStack(FGameplayTag StatTag, int32 StackCount)
	{
		RemoveStatTagStack(StatTag, StackCount);
	}

	/*
	 Get Stat Tag Stack Count
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Item|TagStacks", meta = (DisplayName = "Get Stat Tag Stack Count"))
	UPARAM(DisplayName = "Stack Count") int32 K2_GetStatTagStackCount(FGameplayTag StatTag) const
	{
		return GetStatTagStackCount(StatTag);
	}

	/*
	 Has Tag Stack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Item|TagStacks", meta = (DisplayName = "Has Tag Stack"))
	UPARAM(DisplayName = "Has Tag Stack") bool K2_HasTagStack(FGameplayTag StatTag) const
	{
		return HasTagStack(StatTag);
	}
#pragma endregion

private:
	friend struct FCommonInventoryList;

	// Item Definition of this Instance
	UPROPERTY(Replicated)
	TObjectPtr<UCommonItemDefinition> ItemDefinition;

	// Tag Stacks of this Instance
	UPROPERTY(Replicated)
	FGameplayTagStackContainer TagStacks;

	// Owner Component of this Instance
	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};
