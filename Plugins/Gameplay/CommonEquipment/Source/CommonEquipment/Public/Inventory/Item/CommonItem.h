// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonItem.generated.h"

UCLASS(Abstract, HideCategories = (Replication))
class COMMONEQUIPMENT_API ACommonItem : public AActor
{
	GENERATED_BODY()

public:
	ACommonItem();

	//~ Begin ACommonItem Contract
	void SetOwningPawn(APawn* NewOwner);
	APawn* GetOwningPawn() const;
	virtual void OnEquipped(APawn* NewOwner);
	virtual void OnUnequipped();
	//~ End ACommonItem Contract

	/***********************************************************************************************************************
	* BLUEPRINT FUNCTIONS
	**********************************************************************************************************************/

	/*
	 On Equipped Event
	 Called when the item is equipped
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|Item", meta = (DisplayName = "On Equipped"))
	void K2_OnEquipped(APawn* NewOwner);

	/*
	 On Unequipped Event
	 Called when the item is unequipped
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|Item", meta = (DisplayName = "On Unequipped"))
	void K2_OnUnequipped(APawn* OldOwner);

private:
	UPROPERTY(ReplicatedUsing = OnRep_OwningPawn)
	TObjectPtr<APawn> OwningPawn;

	UFUNCTION()
	void OnRep_OwningPawn(APawn* PreviousOwner);
};
