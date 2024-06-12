// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonEquipmentBaseComponent.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "CommonInventoryComponent.generated.h"

class UInventoryConfig;
struct FBlueprintQuickBarSlotInitializer;
class UCommonItemDefinition;
class UCommonInventoryItemInstance;
struct FCommonInventoryList;
class UCommonInventoryComponent;

/**
 * A message that is broadcast when the inventory changes
 */
USTRUCT(BlueprintType)
struct FCommonInventoryChangedMessage
{
	GENERATED_BODY()

	/**
	 * Owner Component of the Inventory
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	/**
	 * The Item Instance that changed
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UCommonInventoryItemInstance> Instance = nullptr;

	/**
	 * The new count of the Item Instance
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 NewCount = 0;

	/**
	 * The change in count of the Item Instance
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 Delta = 0;
};


/**
 * A single inventory entry in the inventory
 */
USTRUCT(BlueprintType)
struct FCommonInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCommonInventoryEntry()
	{
	}

	FCommonInventoryEntry(UCommonInventoryItemInstance* InItemInstance, int32 InStackCount)
		: Instance(InItemInstance)
		, StackCount(InStackCount)
	{
	}

	bool IsValid() const
	{
		return Instance != nullptr;
	}

private:
	friend UCommonInventoryComponent;
	friend FCommonInventoryList;

	/*
	 Instance
	 The Item Instance for this entry
	 */
	UPROPERTY()
	TObjectPtr<UCommonInventoryItemInstance> Instance = nullptr;

	/*
	 Stack Count
	 The number of items in the stack
	 */
	UPROPERTY()
	int32 StackCount = 0;

	/*
	 Last Observed Stack Count
	 The last observed stack count for this entry
	 */
	UPROPERTY(NotReplicated)
	int32 LastObservedStackCount = INDEX_NONE;

	static FCommonInventoryEntry EmptyEntry;
};

/**
 * A list of inventory entries in the inventory
 */
USTRUCT(BlueprintType)
struct FCommonInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FCommonInventoryList()
	{
	}

	explicit FCommonInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}
	
	//~Begin FCommonInventoryList Contract
	TArray<UCommonInventoryItemInstance*> GetAllItems() const;
	FCommonInventoryEntry& GetEntry(UCommonInventoryItemInstance* ItemInstance);
	FCommonInventoryEntry& GetEntry(UCommonItemDefinition* ItemDefinition);
	UCommonInventoryItemInstance* AddEntry(UCommonItemDefinition* ItemDefinition, int32 StackCount, bool& bIsNewEntry);
	UCommonInventoryItemInstance* AddEntry(UCommonItemDefinition* ItemDefinition, int32 StackCount, FGameplayTagContainer CreationContext, bool& bIsNewEntry);
	void RemoveEntry(UCommonInventoryItemInstance* ItemInstance, int32 StackCount, int32& StackCountAfter);
	//~End FCommonInventoryList Contract

	//~Begin FFastArraySerializer Contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End FFastArraySerializer Contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FCommonInventoryEntry, FCommonInventoryList>(Entries, DeltaParams, *this);
	}

private:
	friend UCommonInventoryComponent;

	void BroadcastChangeMessage(FCommonInventoryEntry& Entry, int32 OldCount, int32 NewCount) const;

	/**
	 * The list of inventory entries
	 */
	UPROPERTY()
	TArray<FCommonInventoryEntry> Entries;

	/**
	 * The owner component that owns this inventory list
	 */
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent = nullptr;
};

template<>
struct TStructOpsTypeTraits<FCommonInventoryList> : public TStructOpsTypeTraitsBase2<FCommonInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * Inventory manager component that manages the inventory of an actor
 */
UCLASS(ClassGroup=("Inventory"), meta=(BlueprintSpawnableComponent, DisplayName = "Inventory Manager"), DisplayName = "Inventory Manager", HideCategories = "DevOnly")
class COMMONEQUIPMENT_API UCommonInventoryComponent : public UCommonEquipmentBaseComponent
{
	GENERATED_BODY()

public:
	UCommonInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//~ Begin UCommonInventoryComponent Interface
	virtual void InitInventory(AActor* InOwnerActor, AActor* InAvatarActor) override;
	virtual bool CanAddItemDef(UCommonItemDefinition* ItemDefinition, int32 StackCount) const;
	virtual UCommonInventoryItemInstance* AddItemDef(UCommonItemDefinition* ItemDefinition, int32 StackCount, FGameplayTagContainer CreationContext, bool& bIsNewEntry);
	virtual void RemoveItemInstance(UCommonInventoryItemInstance* ItemInstance, int32 StackCount, int32& StackCountAfter);
	virtual void AddChildComponent(UCommonEquipmentBaseComponent* ChildComponent);
	//~ End UCommonInventoryComponent Interface

	/**
	 * Returns the list of inventory entries in the inventory
	 */
	FCommonInventoryList& GetInventoryList();

	/**
	 * Returns the stack count of the item definition in the inventory
	 */
	int32 GetItemStackCount(UCommonInventoryItemInstance* ItemInstance) const;

	/**
	 * Returns the avatar actor of this inventory
	 */
	AActor* GetAvatarActor() const;

	/**
	 * Returns the child component of the specified type
	 */
	template<typename T>
	T* GetInventoryChildComponent();

	//~ Begin UObject Interface
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	virtual void OnUnregister() override;
	//~ End UObject Interface

	/****************************************************************************************/
	/*		Blueprint Functions																*/
	/****************************************************************************************/

	/**
	 * Adds an item definition to the inventory
	 *
	 * @param ItemDefinition The item definition to add
	 * @param StackCount The number of items to add
	 * @param CreationContext The creation context for the item
	 * @param bIsNewEntry True if the item was a new entry, false otherwise
	 * @return The item instance that was added
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory|Management", meta = (DisplayName = "Add Item Definition"))
	UPARAM(DisplayName = "Item Instance") UCommonInventoryItemInstance* K2_AddItemDef(UCommonItemDefinition* ItemDefinition, int32 StackCount, FGameplayTagContainer CreationContext, bool& bIsNewEntry);

	/**
	 * Removes an item instance from the inventory
	 *
	 * @param ItemInstance The item instance to remove
	 * @param StackCount The number of items to remove
	 * @param StackCountAfter The number of items after the removal
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory|Management", meta = (DisplayName = "Remove Item Instance"))
	void K2_RemoveItemInstance(UCommonInventoryItemInstance* ItemInstance, int32 StackCount, int32& StackCountAfter);

	/**
	 * Returns all items in the inventory
	 *
	 * @return The list of items in the inventory
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Management", meta = (DisplayName = "Get all Items"))
	UPARAM(DisplayName = "Items") TArray<UCommonInventoryItemInstance*> K2_GetAllItems() const;

	/**
	 * Checks if an item definition can be added to the inventory
	 *
	 * @param ItemDefinition The item definition to check
	 * @param StackCount The number of items to add
	 * @return True if the item definition can be added, false otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Management", meta = (DisplayName = "Can Add Item Definition"))
	UPARAM(DisplayName = "Cann Add") bool K2_CanAddItemDef(UCommonItemDefinition* ItemDefinition, int32 StackCount) const;

	/**
	 * Returns the child component of the specified type
	 *
	 * @param ComponentClass The class of the child component to get
	 * @return The child component of the specified type
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeterminesOutputType = "ComponentClass", DisplayName = "Get Inventory Child Component"))
	UCommonEquipmentBaseComponent* K2_GetInventoryChildComponent(TSubclassOf<UCommonEquipmentBaseComponent> ComponentClass);

private:
	/**
	 * The list of inventory entries
	 */
	UPROPERTY(Replicated)
	FCommonInventoryList InventoryList;

	/** Configuration data asset which can be used to expand the inventory's functionality */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInventoryConfig> InventoryConfig;

	UPROPERTY(ReplicatedUsing = OnRep_ChildComponents)
	TArray<UCommonEquipmentBaseComponent*> ChildComponents;

	UFUNCTION()
	void OnRep_ChildComponents();
};

template <typename T>
T* UCommonInventoryComponent::GetInventoryChildComponent()
{
	for (UCommonEquipmentBaseComponent* Component : ChildComponents)
	{
		if (T* CastedComponent = Cast<T>(Component))
		{
			return CastedComponent;
		}
	}

	return nullptr;
}


/****************************************************************************************/
/*		Inline Function Implementations													*/
/****************************************************************************************/

FORCEINLINE FCommonInventoryList& UCommonInventoryComponent::GetInventoryList()
{
	return InventoryList;
}

FORCEINLINE int32 UCommonInventoryComponent::GetItemStackCount(UCommonInventoryItemInstance* ItemInstance) const
{
	for (const FCommonInventoryEntry& Entry : InventoryList.Entries)
	{
		if (Entry.IsValid() && Entry.Instance == ItemInstance)
		{
			return Entry.StackCount;
		}
	}
	return INDEX_NONE;
}

FORCEINLINE AActor* UCommonInventoryComponent::GetAvatarActor() const
{
	return AvatarActor.Get();
}

FORCEINLINE UCommonInventoryItemInstance* UCommonInventoryComponent::K2_AddItemDef(UCommonItemDefinition* ItemDefinition, int32 StackCount, FGameplayTagContainer CreationContext, bool& bIsNewEntry)
{
	return AddItemDef(ItemDefinition, StackCount, CreationContext, bIsNewEntry);
}

FORCEINLINE void UCommonInventoryComponent::K2_RemoveItemInstance(UCommonInventoryItemInstance* ItemInstance, int32 StackCount, int32& StackCountAfter)
{
	RemoveItemInstance(ItemInstance, StackCount, StackCountAfter);
}

FORCEINLINE TArray<UCommonInventoryItemInstance*> UCommonInventoryComponent::K2_GetAllItems() const
{
	return InventoryList.GetAllItems();
}

FORCEINLINE bool UCommonInventoryComponent::K2_CanAddItemDef(UCommonItemDefinition* ItemDefinition, int32 StackCount) const
{
	return CanAddItemDef(ItemDefinition, StackCount);
}