// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BotaniInteractionComponent.generated.h"


UCLASS(ClassGroup=("Interaction"), meta=(BlueprintSpawnableComponent))
class BOTANIBOTS_API UBotaniInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBotaniInteractionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin UBotaniInteractionComponent Contract
	virtual void SetPotentialInteractable(AActor* NewInteractable);
	virtual void InteractionTimer();
	virtual void ScanForInteractables();
	virtual bool TryCommitInteraction();
	//~End UBotaniInteractionComponent Contract

	/***********************************************************************************************************************
	 * BLUEPRINT EXPOSED FUNCTIONS
	 **********************************************************************************************************************/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Interaction", meta = (DisplayName = "Try Commit Interaction"))
	bool K2_TryCommitInteraction() { return TryCommitInteraction(); }

protected:
	virtual void BeginPlay() override;

private:
	//~Begin UBotaniInteractionComponent Contract
	virtual bool CommitInteraction(AActor* Interactable);
	virtual void PerformInteractionTrace(FHitResult& OutHitResult) const;
	void ClearInteractionTimer();
	//~End UBotaniInteractionComponent Contract

	FTimerDelegate InteractionTimerDelegate;
	FTimerHandle InteractionTimerHandle;
	
	UPROPERTY()
	TObjectPtr<APawn> OwningPawn;

	UPROPERTY(ReplicatedUsing = OnRep_PotentialInteractable)
	TObjectPtr<AActor> PotentialInteractable;

	UFUNCTION()
	void OnRep_PotentialInteractable();
};
