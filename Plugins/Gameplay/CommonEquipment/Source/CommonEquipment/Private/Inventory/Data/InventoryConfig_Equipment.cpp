// Copyright © 2024 MajorT. All rights reserved.


#include "Inventory/Data/InventoryConfig_Equipment.h"
#include "Inventory/Component/CommonEquipmentComponent.h"


UInventoryConfig_Equipment::UInventoryConfig_Equipment()
{
	EquipmentComponentClass = UCommonEquipmentComponent::StaticClass();
}

TSubclassOf<UCommonEquipmentBaseComponent> UInventoryConfig_Equipment::GetAdditionalComponentClass() const
{
	return EquipmentComponentClass;
}
