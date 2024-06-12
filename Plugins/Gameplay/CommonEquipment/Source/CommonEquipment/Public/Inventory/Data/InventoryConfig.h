// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InventoryConfig.generated.h"

struct FGameplayTag;
class UCommonItemDefinition;
class UCommonEquipmentBaseComponent;
class UCommonInventoryComponent;
/**
 * Initial inventory configuration fragment used to expand the inventory's functionality
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract, BlueprintType, BlueprintType, DisplayName = "Inventory Config")
class COMMONEQUIPMENT_API UInventoryConfigFragment : public UObject
{
	GENERATED_BODY()

public:
	//~ Begin UInventoryConfigFragment Contract
	virtual void PostInventoryInitialize(UCommonInventoryComponent* InventoryComponent, AActor* InOwnerActor, AActor* InAvatarActor);
	
	virtual TSubclassOf<UCommonEquipmentBaseComponent> GetAdditionalComponentClass() const
	{
		return nullptr;
	}
	//~ End UInventoryConfigFragment Contract
};

/**
 * Initial Items that will be given to the inventory
 */
USTRUCT(BlueprintType)
struct FCommonInitialItemConfig
{
	GENERATED_BODY()

	/** The item that will be given to the inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UCommonItemDefinition> Item;

	/** The count of the item that will be given to the inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 Count = 1;

	/** Should the item be added to the container */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	uint8 bAddToContainer:1;

	/** The container tag to add the item to */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (EditCondition = "bAddToContainer"))
	FGameplayTag ContainerTag;
	
	/** Should the item be equipped when given */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (EditCondition = "bAddToContainer"))
	uint8 bEquipItem:1;
};


/**
 * Stores data for the initial inventory configuration
 */
UCLASS()
class COMMONEQUIPMENT_API UInventoryConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/** List of additional inventory configurations */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Inventory")
	TArray<UInventoryConfigFragment*> InventoryConfigurations;

	/** List of initial items that will be given to the inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TArray<FCommonInitialItemConfig> InitialItems;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
