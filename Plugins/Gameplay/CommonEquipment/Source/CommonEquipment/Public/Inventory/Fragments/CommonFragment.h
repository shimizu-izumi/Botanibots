// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CommonFragment.generated.h"

class UCommonInventoryItemInstance;
/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract, BlueprintType, BlueprintType)
class COMMONEQUIPMENT_API UCommonFragment : public UObject
{
	GENERATED_BODY()

public:
	UCommonFragment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//~Begin UCommonFragment Contract
	virtual void OnInstanceCreated(UCommonInventoryItemInstance* ItemInstance, const FGameplayTagContainer& CreationContext);
	virtual void OnInstanceDestroyed(UCommonInventoryItemInstance* ItemInstance);
	virtual bool IsFragmentValidForInstance(UCommonInventoryItemInstance* ItemInstance) const;
	virtual bool IsFragmentValidForContext(const FGameplayTagContainer& CreationContext) const;
	virtual bool OnlyForInitialInstance() const;
	//~End UCommonFragment Contract

	/*
	 On Instance Created
	 Called when an instance of the item is created
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "On Instance Created"), Category = "Item Fragment")
	void K2_OnInstanceCreated(UCommonInventoryItemInstance* ItemInstance, const FGameplayTagContainer& CreationContext);

	/*
	 On Instance Destroyed
	 Called when an instance of the item is destroyed
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "On Instance Destroyed"), Category = "Item Fragment")
	void K2_OnInstanceDestroyed(UCommonInventoryItemInstance* ItemInstance);

	/*
	 Is Fragment Valid For Instance
	 Checks if the fragment is valid for the given instance and should be applied
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Is Fragment Valid For Instance"), Category = "Item Fragment")
	bool K2_IsFragmentValidForInstance(UCommonInventoryItemInstance* ItemInstance) const;

	/*
	 Is Fragment Valid For Context
	 Checks if the fragment is valid for the given context and should be applied
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Is Fragment Valid For Context"), Category = "Item Fragment")
	bool K2_IsFragmentValidForContext(const FGameplayTagContainer& CreationContext) const;

	/*
	 Only For Initial Instance
	 Checks if the fragment should only be applied to the initial instance of the item
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Only For Initial Instance"), Category = "Item Fragment")
	bool K2_OnlyForInitialInstance() const;
};
