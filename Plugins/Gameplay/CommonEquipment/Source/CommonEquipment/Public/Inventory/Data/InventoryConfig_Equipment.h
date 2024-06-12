// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryConfig.h"
#include "InventoryConfig_Equipment.generated.h"

class UCommonEquipmentComponent;

/**
 * Config for an additional equipment component that manages equipping and un-equipping items
 */
UCLASS()
class COMMONEQUIPMENT_API UInventoryConfig_Equipment : public UInventoryConfigFragment
{
	GENERATED_BODY()

public:
	UInventoryConfig_Equipment();
	
	//~ Begin UInventoryConfigFragment Contract
	virtual TSubclassOf<UCommonEquipmentBaseComponent> GetAdditionalComponentClass() const override;
	//~ End UInventoryConfigFragment Contract

protected:
	/** Equipment component class to use for this inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TSubclassOf<UCommonEquipmentComponent> EquipmentComponentClass;
};
