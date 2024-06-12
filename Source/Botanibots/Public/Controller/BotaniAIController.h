// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotaniAIController.generated.h"

UCLASS()
class BOTANIBOTS_API ABotaniAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABotaniAIController();

	//~ Begin IGenericTeamAgentInterface Interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;


	UFUNCTION(BlueprintNativeEvent)
	void OnPawnDying();

	UFUNCTION(BlueprintNativeEvent)
	void OnPawnSpawning();
	

private:
	UFUNCTION()
	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(ReplicatedUsing=OnRep_OwningTeamID)
	FGenericTeamId OwningTeamID;

	UFUNCTION()
	virtual void OnRep_OwningTeamID(FGenericTeamId OldTeamID);
};
