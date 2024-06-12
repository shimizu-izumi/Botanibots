// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonItemDefinition.h"
#include "CommonEquippableItemDefinition.generated.h"

/**
 * 
 */
UCLASS()
class COMMONEQUIPMENT_API UCommonEquippableItemDefinition : public UCommonItemDefinition
{
	GENERATED_BODY()

public:
	UCommonEquippableItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Item|Equipment")
	const FCommonEquipmentData& GetEquipmentData() const;
	
	/***********************************************************************************************************************
	* ITEM PROPERTIES
	**********************************************************************************************************************/

	/*
	Equipment Data
	Stores data about the Item that can be equipped
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Functionality|Equipment")
	FCommonEquipmentData EquipmentData;
};
