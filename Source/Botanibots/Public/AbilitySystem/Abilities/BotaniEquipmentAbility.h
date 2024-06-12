// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BotaniAbility.h"
#include "BotaniEquipmentAbility.generated.h"

class UCommonInventoryItemInstance;
/**
Botani Equipment Ability
An ability that is granted by and associated with an inventory item instance
 */
UCLASS()
class BOTANIBOTS_API UBotaniEquipmentAbility : public UBotaniAbility
{
	GENERATED_BODY()

public:
	UBotaniEquipmentAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Botani|Ability")
	UCommonInventoryItemInstance* GetAssociatedItem() const;

	UFUNCTION(BlueprintCallable, Category = "Botani|Ability", meta = (DeterminesOutputType = "ItemClass"))
	UCommonInventoryItemInstance* GetAssociatedItemOfClass(TSubclassOf<UCommonInventoryItemInstance> ItemClass) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
