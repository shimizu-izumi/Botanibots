// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CommonEquipmentBaseComponent.generated.h"


UCLASS(Abstract)
class COMMONEQUIPMENT_API UCommonEquipmentBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCommonEquipmentBaseComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * @brief Initializes the Inventory.
	 * 
	 * Same concept as the InitAbilityActorInfo of the Ability System Component.
	 * It has an owner actor, which represents the actor that owns and manages the inventory. And an avatar actor, which represents the actor that is using the inventory and equipping items (if set).
	 * Should be called after the inventory component is created and before it is used, otherwise the inventory will not work correctly.
	 * @param InOwnerActor The actor that owns this inventory component
	 * @param InAvatarActor The actor that is currently using this inventory
	 */
	virtual void InitInventory(AActor* InOwnerActor, AActor* InAvatarActor);

	
	/**
	 * Owner actor that owns this inventory component
	 */
	UPROPERTY(Replicated)
	TObjectPtr<AActor> OwnerActor;

	/**
	 * Avatar actor that is currently using this inventory
	 */
	UPROPERTY(Replicated)
	TObjectPtr<AActor> AvatarActor;
};
