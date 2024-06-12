// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Definition/CommonItemDefinitionData.h"
#include "Inventory/Fragments/CommonItemFragment.h"
#include "BotaniWeaponFragment.generated.h"

enum class ECommonItemType : uint8;
/*
 Weapon Fragment
 Base class for all weapon fragments, which will represent a weapon in the game
 */
UCLASS(Abstract)
class BOTANIBOTS_API UBotaniWeaponFragment : public UCommonFragment
{
	GENERATED_BODY()

public:
	UBotaniWeaponFragment();
	
	/*
	 Weapon Type
	 The type of weapon this fragment represents
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data")
	ECommonItemType WeaponType = ECommonItemType::None;
};
