// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagStacks/GameplayTagStackContainer.h"
#include "NiagaraSystem.h"
#include "CommonItemDefinitionData.generated.h"


class ACommonItem;

/*
 Release Type
 */
UENUM(BlueprintType)
enum class ECommonReleaseType : uint8
{
	Unset,
	Released,
	ComingSoon,
};

/*
 Blueprint Initial Tag Stacks
 */
USTRUCT(BlueprintType)
struct FBlueprintInitialTagStacks
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stacks")
	FGameplayTag GameplayTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stacks")
	int32 StackCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stacks")
	bool bIsPersistent = false;
};

/*
 Item Stacking Data
 */
USTRUCT(BlueprintType)
struct FCommonItemStackingData
{
	GENERATED_BODY()

	/*
	 Supports Stacking
	 If the Item supports stacking, it will be added to the same stack in the Inventory
	 If the Item does not support stacking, it will be added as a new single stack in the Inventory
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stacking")
	bool bSupportsStacking = false;

	/*
	 Max Stack Size
	 The maximum number of Items that can be in a single stack
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stacking", meta = (EditCondition = "bSupportsStacking"))
	int32 MaxStackSize = 1;
};

/*
 Equipment Requirements
 */
USTRUCT(BlueprintType)
struct FCommonEquipmentRequirements
{
	GENERATED_BODY()

	/*
	 Target Required Tags
	 The Tags that the Target must have to equip this Item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality")
	FGameplayTagContainer TargetRequiredTags;

	/*
	 Target Blocked Tags
	 The Tags that the Target must not have to equip this Item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality")
	FGameplayTagContainer TargetBlockedTags;

	/*
	 Required Item Tags
	 Items that must be in the Inventory of the Target to equip this Item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality")
	FGameplayTagContainer RequiredItemTags;

	/*
	 Blocked Item Tags
	 Items that must not be in the Inventory of the Target to equip this Item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality")
	FGameplayTagContainer BlockedItemTags;

	/*
	 Stacking Data
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality")
	FCommonItemStackingData StackingData;
};

/*
 Auto Pickup Behavior
 Describes the Behavior of the Item when the Player walks over it
 */
UENUM(BlueprintType)
enum class ECommonAutoPickupBehavior : uint8
{
	/*
	 Do Not Auto Pickup
	 The Item will not be picked up automatically
	 */
	DoNotAutoPickup,

	/*
	 Auto Pickup If Has Empty Slot
	 The Item will be picked up automatically if the Player has an empty slot in their Inventory
	 */
	AutoPickupIfHasEmptySlot,

	/*
	 Auto Pickup Replace Current Stack
	 The Item will be picked up automatically and replace the current stack in the Inventory
	 */
	AutoPickupReplaceCurrentStack,

	/*
	 Auto Pickup Replace First Stack
	 The Item will be picked up automatically and replace the first stack in the Inventory
	 */
	AutoPickupReplaceFirstStack,
};

/*
 Pickup Data
 Stores data about the Pickup Behavior of the Item
 */
USTRUCT(BlueprintType)
struct FCommonPickupData
{
	GENERATED_BODY()

	/*
	 Auto Equip
	 Should the Item be automatically equipped when picked up
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup")
	bool bAutoEquip = true;

	/*
	 Auto Pickup
	 Should the Item be automatically picked up when the Player walks over it
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	ECommonAutoPickupBehavior AutoPickup = ECommonAutoPickupBehavior::DoNotAutoPickup;

	/*
	 Destroy on Pickup
	 Should the Item Pickup which is placed in the world be destroyed after being picked up
	 This is useful for Items that can only be picked up once
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	bool bDestroyOnPickup = true;

	/*
	 Pickup Collision Radius
	 The radius of the collision that will be used to detect any possible interactions
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	float PickupCollisionRadius = 50.f;

	/*
	 Pickup Display Mesh
	 The Mesh that will be displayed when the Item is placed in the world
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	TSoftObjectPtr<UStaticMesh> PickupDisplayMesh;

	/*
	 Element Tag
	 The Tag that will be used to identify the Element of the Item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	FGameplayTag ElementTag = FGameplayTag::EmptyTag;

	/*
	 Element Data Table
	 The Data Table that will be used to look up the Data for the corresponding Element
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	TObjectPtr<UDataTable> ElementDataTable;

	/*
	 Pickup Relative Transform
	 The Relative Transform that will be used to place the Pickup Display Mesh in the world
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	FTransform PickupRelativeTransform;

	/*
	 Pickup Effect
	 The Effect that will be played when the Item is picked up
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSoftClassPtr<UNiagaraSystem> PickupEffect;

	/*
	 Pickup Sound
	 The Sound that will be played when the Item is picked up
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	USoundBase* PickupSound;

	/*
	 Drop Sound
	 The Sound that will be played when the Item is dropped
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	USoundBase* DropSound;
};

/*
 Drop Behavior
 Describes the Behavior of the Item when the Player drops it
 */
UENUM(BlueprintType)
enum class ECommonDropBehavior : uint8
{
	/*
	 Do Not Drop
	 The Item cannot be dropped
	 */
	DontDrop,

	/*
	 Drop as Pickup
	 The Item will be dropped as a Pickup in the world
	 */
	DropAsPickup,

	/*
	 Destroy on Drop
	 The Item will be destroyed when dropped
	 */
	DestroyOnDrop,

	/*
	 Drop as Pickup and Destroy if Empty
	 The Item will be dropped as a Pickup in the world and destroyed if the stack is empty
	 */
	DropAsPickupAndDestroyIfEmpty,
};

/*
 Item Type
 Describes the Type of the Item
 */
UENUM(BlueprintType)
enum class ECommonItemType : uint8
{
	/*
	 None
	 */
	None,

	/*
	 Consumable
	 An Item that can be consumed to provide a temporary effect
	 */
	Consumable,

	/*
	 Weapon Ranged
	 A Ranged Weapon that can be used to attack enemies from a distance
	 */
	WeaponRanged,

	/*
	 Weapon Melee
	 A Melee Weapon that can be used to attack enemies in close combat
	 */
	WeaponMelee,

	/*
	 Weapon Hybrid
	 A Hybrid Weapon that has multiple attack modes
	 */
	WepaonHybrid,

	/*
	 Weapon Mod
	 A Weapon Modification that can be attached to a Weapon to provide additional effects
	 */
	WeaponMod,

	/*
	 Throwable
	 An Item that can be thrown to deal damage to enemies
	 */
	Throwable,

	/*
	 Ammo
	 An Item that can be used to reload a Weapon
	 */
	Ammo,
};

/*
 Equipment Data
 Stores data about the Item that can be equipped
 */
USTRUCT(BlueprintType)
struct FCommonEquipmentData
{
	GENERATED_BODY()

	/*
	 Equipment Actor
	 The Actor that will be spawned when the Item is equipped
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSoftClassPtr<ACommonItem> EquipmentActor;

	/*
	 Attachment Socket
	 The Socket that will be used to attach the Equipment Actor to the Pawn
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FName AttachmentSocket = NAME_None;

	/*
	 Equipment Relative Transform
	 The Relative Transform that will be used to place the Equipment Actor on the Pawn
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FTransform RelativeTransform = FTransform::Identity;
};


UCLASS()
class UCommonItemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Equipment|Initial")
	static FGameplayTagStack TagStackFromBlueprintData(const FBlueprintInitialTagStacks& Data);
};

inline FGameplayTagStack UCommonItemBlueprintLibrary::TagStackFromBlueprintData(const FBlueprintInitialTagStacks& Data)
{
	FGameplayTagStack NewStack(Data.GameplayTag, FGameplayTagStackValue(Data.StackCount, Data.bIsPersistent));
	return NewStack;
}
