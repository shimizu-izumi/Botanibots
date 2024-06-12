// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Inventory/Definition/CommonItemDefinition.h"

#include "Inventory/Instance/CommonInventoryItemInstance.h"

UCommonItemDefinition::UCommonItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemType = ECommonItemType::None;
	PickupData = FCommonPickupData();
}

FPrimaryAssetId UCommonItemDefinition::GetPrimaryAssetId() const
{
	if (ReleaseType == ECommonReleaseType::Released)
	{
		return FPrimaryAssetId("Item", *GetName());
	}
	return FPrimaryAssetId("UnreleasedItem", *GetName());
}

bool UCommonItemDefinition::CanBeDropped() const
{
	return DropBehavior != ECommonDropBehavior::DontDrop;
}

TSubclassOf<UCommonInventoryItemInstance> UCommonItemDefinition::GetItemInstanceClass() const
{
	return UCommonInventoryItemInstance::StaticClass();
}
