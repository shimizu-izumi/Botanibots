// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Actor.h"
#include "Pickup/Interface/CommonInteractionInterface.h"
#include "Pickup/Interface/CommonPickupInterface.h"
#include "CommonPickupProxy.generated.h"

class UWidgetComponent;
class UTagStackComponent;
class USphereComponent;
class UCommonItemDefinition;

/**
 * Represents a pickup item in the world.
 * Can be used to be dropped, picked-up and interacted with.
 * Just a proxy of the pickup item, not the actual item.
 *
 * Supports replication, saving and loading stacks.
 */
UCLASS(Abstract, DisplayName = "Pickup Proxy Actor")
class COMMONEQUIPMENT_API ACommonPickupProxy : public AActor, public ICommonInteractionInterface, public ICommonPickupInterface
{
	GENERATED_BODY()

public:
	ACommonPickupProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region interfaces
	//~Begin ICommonInteractionInterface Interface
	/**
	 * Returns whether the given actor can interact with this interactable.
	 * Can be defined by the implementer.
	 * @param InteractingActor	The actor that is interacting with this interactable.
	 */
	virtual bool CanInteract(AActor* InteractingActor) const override;

	/**
	 * Commits the interaction with the given actor.
	 * @param InteractingActor	The actor that is interacting with this interactable.
	 */
	virtual bool CommitInteraction(AActor* InteractingActor) override;

	/**
	 * Touches the interactable with the given actor.
	 * Used to show the interaction prompt.
	 * @param InteractingActor	The actor that is interacting with this interactable. 
	 */
	virtual void TouchInteractable(AActor* InteractingActor) override;

	/**
	* Leaves the interactable with the given actor.
	* Used to hide the interaction prompt.
	* @param InteractingActor	The actor that is interacting with this interactable.
	*/
    virtual void LeaveInteractable(AActor* InteractingActor) override;
	//~End ICommonInteractionInterface Interface

	//~Begin ICommonPickupInterface Interface
	/**
	 * Returns the item definition of this pickup.
	 */
	virtual UCommonItemDefinition* GetItemDefinition() const override;

	/**
	 * Called when the pickup is dropped in the world.
	 * @param ItemDefinition	The item definition of the pickup.
	 */
	virtual void OnPickupDropped(UCommonItemDefinition* ItemDefinition) override;
	//~End ICommonPickupInterface Interface
#pragma endregion

#pragma region blueprint_functions
	/**
	* Returns whether the given actor can interact with this interactable.
	* Can be defined by the implementer.
	* @param InteractingActor	The actor that is interacting with this interactable.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction", meta = (DisplayName = "Can Interact"))
	bool K2_CanInteract(AActor* InteractingActor) const;

	/**
	 * Commits the interaction with the given actor.
	 * @param InteractingActor	The actor that is interacting with this interactable.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "Commit Interaction"))
	void K2_CommitInteraction(AActor* InteractingActor);

	/**
	 * Touches the interactable with the given actor.
	 * Used to show the interaction prompt.
	 * @param InteractingActor	The actor that is interacting with this interactable.
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta = (DisplayName = "Touch Interactable"))
	void K2_TouchInteractable(AActor* InteractingActor);

	/**
	 * Leaves the interactable with the given actor.
	 * Used to hide the interaction prompt.
	 * @param InteractingActor	The actor that is interacting with this interactable.
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta = (DisplayName = "Leave Interactable"))
	void K2_LeaveInteractable(AActor* InteractingActor);

	/**
	 * Called when the pickup is dropped in the world.
	 * @param ItemDefinition	The item definition of the pickup.
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "Pickup", meta = (DisplayName = "On Pickup Dropped"))
	void K2_OnPickupDropped(UCommonItemDefinition* ItemDefinition);
#pragma endregion

protected:
	//~Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	//~End AActor Interface

	//~Begin ACommonPickupProxy Interface
	/**
	 * Setup the pickup proxy with the given item definition.
	 * Loads the display mesh and sets up the tags.
	 */
	virtual void SetupPickupProxy(UCommonItemDefinition* NewItemDefinition);

	/**
	 * Resets the pickup proxy to its default state.
	 * Usually called when the pickup is picked-up.
	 */
	virtual void ResetPickupProxy();

	/**
	 * Shows the display mesh when loaded.
	 */
	virtual void OnDisplayMeshLoaded();

	/**
	 * Updates the current item definition
	 * @param NewItemDefinition	The new item definition to update to.
	 */
	virtual void SetItemDefinition(UCommonItemDefinition* NewItemDefinition);

	/**
	 * Updates the visibility of the prompt widget.
	 */
	virtual void ShowInteractionPrompt(const bool bShow);
	//~End ACommonPickupProxy Interface
	
#pragma region components
	/**
	 * Represents the display mesh of the item proxy.
	 * Lightweight version of the actual item, so that we dont need to load the item into system memory.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PickupDisplayMeshComponent;

	/**
	 * Used to detect overlaps or line traces for the pickup.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> PickupHitboxComponent;

	/**
	 * Responsible for storing and managing the tags of the pickup.
	 * On pickup, the tags will be granted to the target inventory if certain conditions are met.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTagStackComponent> TagStackComponent;

	/**
	 * The prompt to display when the player has the ability to pick up the item.
	 * Will be displayed above the item, and shows details about the item, such as name and description, etc.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> PromptWidgetComponent;
		
#pragma endregion

protected:
	/**
	 * The item definition of the pickup.
	 * Will be used to create and grant the actual item when picked up to the target inventory.
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PickupProxy")
	UCommonItemDefinition* GetCachedItemDefinition() const { return CachedItemDefinition; }
	
	/**
	 * The item definition of the pickup.
	 * Will be used to create and grant the actual item when picked up to the target inventory.
	 */
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_ItemDefinition, Category = "PickupProxy")
	TObjectPtr<UCommonItemDefinition> CachedItemDefinition;

	/**
	 * The color of the pickup element.
	 * Used to tint the pickup display mesh and other cosmetic elements.
	 */
	FLinearColor ElementColor;

private:
	UFUNCTION()
	void OnRep_ItemDefinition();


#pragma region asset_loading
	/**
	 * Unloads the pickup object from memory.
	 */
	virtual void UnloadObjects();
	
	TArray<FSoftObjectPath> LoadedObjects;
	FStreamableManager AssetLoader;
#pragma endregion
};
