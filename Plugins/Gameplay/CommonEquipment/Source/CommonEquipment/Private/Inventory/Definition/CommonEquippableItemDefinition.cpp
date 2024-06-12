// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Inventory/Definition/CommonEquippableItemDefinition.h"

UCommonEquippableItemDefinition::UCommonEquippableItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const FCommonEquipmentData& UCommonEquippableItemDefinition::GetEquipmentData() const
{
	return EquipmentData;
}
