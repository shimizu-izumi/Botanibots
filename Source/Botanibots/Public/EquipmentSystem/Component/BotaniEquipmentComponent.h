// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Component/CommonEquipmentComponent.h"
#include "BotaniEquipmentComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOTANIBOTS_API UBotaniEquipmentComponent : public UCommonEquipmentComponent
{
	GENERATED_BODY()

public:
	UBotaniEquipmentComponent();

	//~Begin UCommonEquipmentComponent Contract
	virtual void SpawnEquipmentActors(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance, USceneComponent* AttachmentTargetOverride = nullptr) override;
	//~End UCommonEquipmentComponent Contract

protected:
	//~Begin UCommonEquipmentComponent Contract
	virtual void OnRep_SpawnedActors() override;
	//~End UCommonEquipmentComponent Contract
};
