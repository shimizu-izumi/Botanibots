// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Controller/BotaniAIController.h"

#include "BrainComponent.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Perception/AIPerceptionComponent.h"


ABotaniAIController::ABotaniAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	OwningTeamID = FGenericTeamId::NoTeam;
	bStopAILogicOnUnposses = false;
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
}

void ABotaniAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Pawn is spawned or respawned
	if (BrainComponent->IsRunning())
	{
		BrainComponent->RestartLogic();
	}
	else
	{
		BrainComponent->StartLogic();
	}

	OnPawnSpawning();
}



void ABotaniAIController::OnPawnDying_Implementation()
{

}

void ABotaniAIController::OnPawnSpawning_Implementation()
{
}

void ABotaniAIController::OnUnPossess()
{
	Super::OnUnPossess();

	BrainComponent->StopLogic("Dead");
}

//~ Begin IGenericTeamAgentInterface Interface
void ABotaniAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, OwningTeamID, this);
		OwningTeamID = NewTeamID;
	}
}

FGenericTeamId ABotaniAIController::GetGenericTeamId() const
{
	return OwningTeamID;
}

ETeamAttitude::Type ABotaniAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			const FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();

			if (OtherTeamID.GetId() == GetGenericTeamId().GetId())
			{
				return ETeamAttitude::Friendly;
			}
			return ETeamAttitude::Hostile;
		}
	}
	
	return ETeamAttitude::Neutral;
}

//~ End IGenericTeamAgentInterface Interface


void ABotaniAIController::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
}


void ABotaniAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
}

void ABotaniAIController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, OwningTeamID, SharedParams);
}

void ABotaniAIController::OnRep_OwningTeamID(FGenericTeamId OldTeamID)
{
	//TODO: Implement logic for when the team ID changes, not gameplay relevant for now
}
