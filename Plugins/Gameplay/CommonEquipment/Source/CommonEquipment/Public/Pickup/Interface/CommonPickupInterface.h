// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonPickupInterface.generated.h"

class UCommonItemDefinition;

UINTERFACE()
class COMMONEQUIPMENT_API UCommonPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Used to pickup items in the world.
 * @see ACommonPickupProxy, ICommonInteractionInterface
 */
class COMMONEQUIPMENT_API ICommonPickupInterface
{
	GENERATED_BODY()

public:
	/**
	 * Returns the item definition of this pickup.
	 */
	virtual UCommonItemDefinition* GetItemDefinition() const = 0;

	/**
	 * Called when the pickup is dropped in the world.
	 * @param ItemDefinition	The item definition of the pickup.
	 */
	virtual void OnPickupDropped(UCommonItemDefinition* ItemDefinition) = 0;
};