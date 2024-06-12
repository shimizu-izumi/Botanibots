// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonInteractionInterface.generated.h"

class UCommonItemDefinition;

/**
 * Used to interact with items in the world.
 */
UINTERFACE()
class COMMONEQUIPMENT_API UCommonInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class COMMONEQUIPMENT_API ICommonInteractionInterface
{
	GENERATED_BODY()

public:
	/**
	 * Returns whether the given actor can interact with this interactable.
	 * Can be defined by the implementer.
	 * @param InteractingActor	The actor that is interacting with this interactable.
	 */
	virtual bool CanInteract(AActor* InteractingActor) const = 0;

	/**
	 * Commits the interaction with the given actor.
	 * @param InteractingActor	The actor that is interacting with this interactable.
	 */
	virtual bool CommitInteraction(AActor* InteractingActor) = 0;

	/**
	 * Touches the interactable with the given actor.
	 * Used to show the interaction prompt.
	 * @param InteractingActor	The actor that is interacting with this interactable. 
	 */
	virtual void TouchInteractable(AActor* InteractingActor) = 0;

	/**
	 * Leaves the interactable with the given actor.
	 * Used to hide the interaction prompt.
	 * @param InteractingActor	The actor that is interacting with this interactable.
	 */
	virtual void LeaveInteractable(AActor* InteractingActor) = 0;
};
