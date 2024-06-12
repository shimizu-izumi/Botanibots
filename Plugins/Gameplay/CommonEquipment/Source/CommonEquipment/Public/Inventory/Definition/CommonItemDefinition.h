// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonItemDefinitionData.h"
#include "Engine/DataAsset.h"
#include "Inventory/Fragments/CommonItemFragment.h"
#include "CommonItemDefinition.generated.h"

class UCommonItemFragment;
class UCommonInventoryItemInstance;
/**
 * 
 */
UCLASS(BlueprintType, Abstract, HideCategories = ("Dev"))
class COMMONEQUIPMENT_API UCommonItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCommonItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin UPrimaryDataAsset Contract
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	//~End UPrimaryDataAsset Contract

	template<typename T>
	T* GetItemFragment() const
	{
		for (UCommonItemFragment* Fragment : ItemFragments)
		{
			if (T* CastedFragment = Cast<T>(Fragment))
			{
				return CastedFragment;
			}
		}
		return nullptr;
	}

	//~Begin UCommonItemDefinition Contract
	virtual bool CanBeDropped() const;
	virtual TSubclassOf<UCommonInventoryItemInstance> GetItemInstanceClass() const;
	//~End UCommonItemDefinition Contract

#pragma region ItemProperties

	/*
	 Release Type
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Release")
	ECommonReleaseType ReleaseType = ECommonReleaseType::Unset;

	/*
	Item Name
	The name of the item that will be displayed in the UI and in the world
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText ItemName;

	/*
	Item Description
	The description of the item that will be displayed in the UI
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")	
	FText ItemDescription;

	/*
	Item Icon
	The icon of the item that will be displayed in the UI
	EDITOR: This texture will be used as the class thumbnail
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	/*
	Item Tag
	The tag that will be used to identify the item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FGameplayTag ItemTag;

	/*
	 Item Type
	 The type of the item that will be used to categorize the item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality", meta = (EditConditionHides, EditCondition = "bCanEditFunctionality"))
	ECommonItemType ItemType;

	/*
	 Is World Item
	 Is the item a world item that can be picked up
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality", meta = (EditConditionHides, EditCondition = "bCanEditFunctionality"))
	bool bIsWorldItem = true;
	
	/*
	 Equipment Requirements
	 The requirements that need to be met in order to use, pickup or equip the item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality", meta = (EditConditionHides, EditCondition = "bCanEditFunctionality"))
	FCommonEquipmentRequirements EquipmentRequirements;

	/*
	 Pickup Data
	 The data that will be used to define the pickup behavior of the item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality|Pickup", meta = (EditConditionHides, EditCondition = "bIsWorldItem"))
	FCommonPickupData PickupData;

	/*
	 Drop on Death
	 Should the item be dropped when the owner dies
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality|Drop", meta = (EditConditionHides, EditCondition = "bIsWorldItem"))
	bool bDropOnDeath = true;

	/*
	 Drop Behavior
	 The behavior that will be used to determine how the item will be dropped
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality|Drop", meta = (EditConditionHides, EditCondition = "bIsWorldItem"))
	ECommonDropBehavior DropBehavior = ECommonDropBehavior::DropAsPickup;

	/*
	 Default Pickup Class Override
	 The class that will be used to override the default pickup class
	 Can also be overridden from Blueprints
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality|Pickup", meta = (EditConditionHides, EditCondition = "bIsWorldItem"))
	TSoftClassPtr<AActor> DefaultPickupClassOverride; //TODO: PickupActor machen

	/*
	 Item Fragments
	 List of fragments that can be used to extend the functionality of the item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Fragments")
	TArray<TObjectPtr<UCommonItemFragment>> ItemFragments;

#pragma endregion

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = "Dev")
	bool bCanEditFunctionality = true;
#endif
};
