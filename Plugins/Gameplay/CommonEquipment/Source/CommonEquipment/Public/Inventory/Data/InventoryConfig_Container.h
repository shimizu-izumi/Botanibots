// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryConfig.h"
#include "InventoryConfig_Container.generated.h"

class UInventoryContainerInitializer;
class UCommonInventoryContainerComponent;
/**
 * Config for an additional container component that manages item storage
 */
UCLASS()
class COMMONEQUIPMENT_API UInventoryConfig_Container : public UInventoryConfigFragment
{
	GENERATED_BODY()

public:
	UInventoryConfig_Container();

	//~ Begin UInventoryConfigFragment Contract
	virtual TSubclassOf<UCommonEquipmentBaseComponent> GetAdditionalComponentClass() const override;
	virtual void PostInventoryInitialize(UCommonInventoryComponent* InventoryComponent, AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~ End UInventoryConfigFragment Contract

protected:
	/** Container component class to use for this inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TSubclassOf<UCommonInventoryContainerComponent> ContainerComponentClass;

	/** Used to initialize the default containers for this component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UInventoryContainerInitializer> ContainerInitializer;
};
