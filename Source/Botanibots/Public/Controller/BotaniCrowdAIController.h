// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotaniCrowdAIController.generated.h"

UCLASS()
class BOTANIBOTS_API ABotaniCrowdAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABotaniCrowdAIController(const FObjectInitializer& ObjectInitializer  = FObjectInitializer::Get());

	//~ Begin IGenericTeamAgentInterface Interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** AI Perception */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

private:
	UPROPERTY(ReplicatedUsing = OnRep_MyTeamId)
	FGenericTeamId MyTeamId;

	UFUNCTION()
	void OnRep_MyTeamId(const FGenericTeamId& OldTeamId);	
};
