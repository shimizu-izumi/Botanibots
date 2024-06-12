// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonEquipmentBaseComponent.h"
#include "AbilitySystem/CommonAbilitySet.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "CommonEquipmentComponent.generated.h"


class UCommonInventoryItemInstance;

USTRUCT(BlueprintType)
struct FSpawnedActorInfo
{
	GENERATED_BODY();
		
	FGameplayTag ContainerTag;

	UPROPERTY()
	TObjectPtr<AActor> Actor;

	bool operator==(const FSpawnedActorInfo& Other) const
	{
		return ContainerTag == Other.ContainerTag && Actor == Other.Actor;
	}
};

/**
 * A single entry in the equipment list which is applied to an actor
 */
USTRUCT(BlueprintType)
struct FCommonAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCommonAppliedEquipmentEntry() = default;

private:
	friend class UCommonEquipmentComponent;
	friend struct FCommonEquipmentList;

	UPROPERTY()
	TObjectPtr<UCommonInventoryItemInstance> ItemInstance;

	UPROPERTY(NotReplicated)
	FCommonAbilitySet_GrantedHandles GrantedHandles;
};

/**
 * A list of equipment entries that are applied to an actor
 */
USTRUCT(BlueprintType)
struct FCommonEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FCommonEquipmentList() = default;
	
	explicit FCommonEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}
	
	//~ Begin FCommonEquipmentList Interface
	void AddEntry(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* Instance);
	void RemoveEntry(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* Instance);
	//~ End FCommonEquipmentList Interface
	
	//~ Begin FFastArraySerializer Interface
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~ End FFastArraySerializer Interface

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FCommonAppliedEquipmentEntry, FCommonEquipmentList>(Entries, DeltaParams, *this);
	}

private:
	friend UCommonEquipmentComponent;

	UPROPERTY()
	TArray<FCommonAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent = nullptr;
};

/**
 * Equipment Component that manages equipping and un-equipping items
 * Also handles granting and revoking abilities from equipped items
 */
UCLASS(ClassGroup=("Inventory"), DisplayName = "Equipment Manager")
class COMMONEQUIPMENT_API UCommonEquipmentComponent : public UCommonEquipmentBaseComponent
{
	GENERATED_BODY()

public:
	UCommonEquipmentComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin UActorComponent Interface
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~ End UActorComponent Interface

	//~ Begin UCommonEquipmentComponent Interface
	virtual void EquipItem(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance);
	virtual void UnequipItem(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance);
	virtual void SpawnEquipmentActors(const FGameplayTag& ContainerTag, UCommonInventoryItemInstance* ItemInstance, USceneComponent* AttachmentTargetOverride = nullptr);
	virtual void DestroyEquipmentActors(const FGameplayTag& ContainerTag);
	//~ End UCommonEquipmentComponent Interface

protected:
	UPROPERTY(ReplicatedUsing = OnRep_SpawnedActors)
	TArray<FSpawnedActorInfo> SpawnedActors;

	UPROPERTY(Replicated)
	FCommonEquipmentList EquipmentList;

	UFUNCTION()
	virtual void OnRep_SpawnedActors();
};
