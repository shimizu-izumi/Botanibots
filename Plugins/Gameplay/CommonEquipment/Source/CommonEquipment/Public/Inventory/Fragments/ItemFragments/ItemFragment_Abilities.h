// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Fragments/CommonItemFragment.h"
#include "ItemFragment_Abilities.generated.h"

class UCommonAbilitySet;
/**
 * 
 */
UCLASS(DisplayName = "Granted Abilitiy Set")
class COMMONEQUIPMENT_API UItemFragment_Abilities : public UCommonItemFragment
{
	GENERATED_BODY()

public:
	/***********************************************************************************************************************
	* ITEM PROPERTIES
	**********************************************************************************************************************/

	/*
	 Ability Set
	 A Set of Abilities that are granted by this Item when equipped
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TObjectPtr<UCommonAbilitySet> AbilitySet;
};
