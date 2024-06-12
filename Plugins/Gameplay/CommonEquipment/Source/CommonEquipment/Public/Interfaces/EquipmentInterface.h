// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipmentInterface.generated.h"

class UCommonInventoryContainerComponent;
class UCommonEquipmentComponent;
class UCommonInventoryComponent;

// This class does not need to be modified.
UINTERFACE()
class UEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONEQUIPMENT_API IEquipmentInterface
{
	GENERATED_BODY()

public:
	/** Get the Inventory Manager Component */
	virtual UCommonInventoryComponent* GetInventoryComponent() const = 0;

	/** Get the Equipment Component that manages equipping and un-equipping items */
	virtual UCommonEquipmentComponent* GetEquipmentComponent() const = 0;

	/** Get the Inventory Container Component that sorts the items in the inventory */
	virtual UCommonInventoryContainerComponent* GetInventoryContainerComponent() const = 0;
};
