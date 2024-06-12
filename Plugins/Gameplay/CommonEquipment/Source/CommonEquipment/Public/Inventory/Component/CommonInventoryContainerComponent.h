// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonEquipmentBaseComponent.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "CommonInventoryContainerComponent.generated.h"

struct FCommonInventoryContainerEntry;
class UCommonInventoryItemInstance;

USTRUCT(BlueprintType)
struct FBlueprintContainerEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FGameplayTag Tag;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<UCommonInventoryItemInstance*> Items;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 ActiveItemIndex = INDEX_NONE;
};

/**
 * Message that is broadcast when the slots in a container have changed
 */
USTRUCT(BlueprintType)
struct FCommonInventoryContainerSlotsChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<AActor> OwnerActor = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<AActor> AvatarActor = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<FBlueprintContainerEntry> ContainerEntries;
};

/**
 * Message that is broadcast when the active slot in a container has changed
 */
USTRUCT(BlueprintType)
struct FCommonInventoryContainerActiveSlotChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<AActor> AvatarActor = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FGameplayTag ContainerTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 ActiveSlotIndex = INDEX_NONE;
};

/**
 * A single entry in the inventory container list
 */
USTRUCT(BlueprintType)
struct FCommonInventoryContainerEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCommonInventoryContainerEntry() = default;

	explicit FCommonInventoryContainerEntry(const FGameplayTag& InTag, const int32 InMaxNumItems = INDEX_NONE)
		: Tag(InTag)
		, MaxNumItems(InMaxNumItems)
	{
	}


private:
	friend class UCommonInventoryContainerComponent;
	friend struct FCommonInventoryContainerList;

	/**
	 * List of items in this container
	 */
	TArray<TObjectPtr<UCommonInventoryItemInstance>> Items;

	/**
	 * The tag for this container
	 */
	FGameplayTag Tag;

	/**
	 * The maximum number of items that can be stored in this container
	 */
	int32 MaxNumItems = INDEX_NONE;

	/**
	 * The active item index in this container
	 */
	int32 ActiveItemIndex = INDEX_NONE;
};

/**
 * A list of inventory containers
 */
USTRUCT(BlueprintType)
struct FCommonInventoryContainerList : public FFastArraySerializer
{
	GENERATED_BODY()

	FCommonInventoryContainerList() = default;

	explicit FCommonInventoryContainerList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	//~ Begin FCommonInventoryContainerList Interface
	FGameplayTag GetContainerTag(UCommonInventoryItemInstance* ItemInstance) const;
	UCommonInventoryItemInstance* GetItem(const FGameplayTag& ContainerTag, const int32 ItemIndex) const;
	UCommonInventoryItemInstance* GetActiveItem(const FGameplayTag& ContainerTag) const;
	int32 GetActiveItemIndex(const FGameplayTag& ContainerTag) const;
	int32 GetNextAvailableSlot(const FGameplayTag& ContainerTag) const;
	int32 GetItemIndex(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance) const;
	int32 GetNumContainers() const;
	int32 GetNumItems(const FGameplayTag& ContainerTag) const;
	int32 AddEntry(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance);
	void RemoveEntry(const FGameplayTag& ContainerTag, const int32 ItemIndex);
	void RemoveEntry(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance);
	void SetActiveSlot(const FGameplayTag& ContainerTag, const int32 ItemIndex);
	//~ End FCommonInventoryContainerList Interface

	//~ Begin FFastArraySerializer Contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~ End FFastArraySerializer Contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FCommonInventoryContainerEntry, FCommonInventoryContainerList>(ContainerEntries, DeltaParams, *this);
	}
	
private:
	friend UCommonInventoryContainerComponent;

	/**
	 * List of replicated inventory container entries
	 */
	UPROPERTY()
	TArray<FCommonInventoryContainerEntry> ContainerEntries;

	/**
	 * Owner component that owns this list
	 */
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent = nullptr;
};

/**
 * Manages the inventory for an actor, storing lists of items into separate containers which can be accessed by a tag.
 */
UCLASS(DisplayName = "Inventory Container Component")
class COMMONEQUIPMENT_API UCommonInventoryContainerComponent : public UCommonEquipmentBaseComponent
{
	GENERATED_BODY()

public:
	UCommonInventoryContainerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin UCommonEquipmentBaseComponent Interface
	virtual int32 AddItemToContainer(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance);
	virtual void RemoveItemFromContainer(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance);
	virtual void RemoveItemFromContainer(const FGameplayTag& ContainerTag, int32 ItemIndex);
	virtual void SetActiveItemInContainer(const FGameplayTag& ContainerTag, int32 ItemIndex);
	virtual UCommonInventoryItemInstance* GetActiveItemInContainer(const FGameplayTag& ContainerTag) const;
	virtual UCommonInventoryItemInstance* GetItemInContainer(const FGameplayTag& ContainerTag, int32 ItemIndex) const;
	virtual TArray<UCommonInventoryItemInstance* > GetAllItemsInContainer(const FGameplayTag& ContainerTag) const;
	virtual int32 GetActiveItemIndexInContainer(const FGameplayTag& ContainerTag) const;
	virtual int32 GetNextAvailableIndexInContainer(const FGameplayTag& ContainerTag) const;
	virtual FGameplayTag GetContainerTag(UCommonInventoryItemInstance* ItemInstance) const;
	//~ End UCommonEquipmentBaseComponent Interface

	//~ Begin UCommonEquipmentBase Interface
	virtual void InitInventory(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~ End UCommonEquipmentBase Interface

	/**
	 * List of initial empty inventory containers
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Container")
	TObjectPtr<class UInventoryContainerInitializer> InitialContainerConfig;


#pragma region blueprint_functions
	UFUNCTION(BlueprintCallable, Category = "Inventory|Container", meta = (DisplayName = "Add Item To Container"))
	int32 K2_AddItemToContainer(FGameplayTag ContainerTag, UCommonInventoryItemInstance* ItemInstance)
	{
		return AddItemToContainer(ContainerTag, ItemInstance);
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory|Container", meta = (DisplayName = "Remove Item From Container"))
	void K2_RemoveItemFromContainerByInstance(FGameplayTag ContainerTag, UCommonInventoryItemInstance* ItemInstance)
	{
		RemoveItemFromContainer(ContainerTag, ItemInstance);
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory|Container", meta = (DisplayName = "Remove Item From Container by Index"))
	void K2_RemoveItemFromContainerByIndex(FGameplayTag ContainerTag, int32 ItemIndex)
	{
		RemoveItemFromContainer(ContainerTag, ItemIndex);
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory|Container", meta = (DisplayName = "Set Active Item In Container"))
	void K2_SetActiveItemInContainer(FGameplayTag ContainerTag, int32 ItemIndex)
	{
		SetActiveItemInContainer(ContainerTag, ItemIndex);
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory|Container", meta = (DisplayName = "Get Active Item In Container"))
	UCommonInventoryItemInstance* K2_GetActiveItemInContainer(FGameplayTag ContainerTag) const
	{
		return GetActiveItemInContainer(ContainerTag);
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory|Container", meta = (DisplayName = "Get Item In Container"))
	UCommonInventoryItemInstance* K2_GetItemInContainer(FGameplayTag ContainerTag, int32 ItemIndex) const
	{
		return GetItemInContainer(ContainerTag, ItemIndex);
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory|Container", meta = (DisplayName = "Get All Items In Container"))
	TArray<UCommonInventoryItemInstance*> K2_GetAllItemsInContainer(FGameplayTag ContainerTag) const
	{
		return GetAllItemsInContainer(ContainerTag);
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory|Container", meta = (DisplayName = "Get Active Item Index In Container"))
	int32 K2_GetActiveItemIndexInContainer(FGameplayTag ContainerTag) const
	{
		return GetActiveItemIndexInContainer(ContainerTag);
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory|Container", meta = (DisplayName = "Get Next Available Index in Container"))
	int32 K2_GetNextAvailableIndexInContainer(FGameplayTag ContainerTag) const
	{
		return GetNextAvailableIndexInContainer(ContainerTag);
	}
#pragma endregion

private:
	//~ Begin UCommonEquipmentBaseComponent Interface
	virtual void EquipItemInContainer(const FGameplayTag& ContainerTag, int32 ItemIndex);
	virtual void UnequipItemInContainer(const FGameplayTag& ContainerTag, int32 ItemIndex);
	//~ End UCommonEquipmentBaseComponent Interface

	TMap<FGameplayTag, UCommonInventoryItemInstance*> EquippedItems;
	
	/**
	 * Replicated list of inventory containers
	 */
	UPROPERTY(ReplicatedUsing = OnRep_ContainerList)
	FCommonInventoryContainerList ContainerList;

	UFUNCTION()
	void OnRep_ContainerList();
};

/***********************************************************************************************************************
 * Quick Bar Slot Initializer
 **********************************************************************************************************************/

USTRUCT(BlueprintType)
struct FBlueprintInventoryContainerInitializer
{
	GENERATED_BODY()

	/**
	 * Gameplay Tag that represents the Container
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Container")
	FGameplayTag ContainerTag;

	/**
	 * Max number of items that can be stored in this Container
	 * Negative values indicate infinite storage
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Container")
	int32 MaxItems = INDEX_NONE;
};

UCLASS(DisplayName = "Initial Inventory Container Config")
class UInventoryContainerInitializer : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * List of initial slots that will be used to initialize the Quick Bar
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
	TArray<FBlueprintInventoryContainerInitializer> InitialContainer;
};