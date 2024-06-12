// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Controller/BotaniCrowdAIController.h"

#include "BrainComponent.h"
#include "Interfaces/GenericTeamAgentBlueprintInterface.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Perception/AIPerceptionComponent.h"

//#include UE_INLINE_GENERATED_CPP_BY_NAME(BotaniCrowdAIController)


ABotaniCrowdAIController::ABotaniCrowdAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	if (AIPerceptionComponent)
	{
		//TODO: Default Values
	}
}

//~ Begin IGenericTeamAgentInterface Interface
void ABotaniCrowdAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamId, this);
		MyTeamId = NewTeamID;
	}
}

FGenericTeamId ABotaniCrowdAIController::GetGenericTeamId() const
{
	return MyTeamId;
}

ETeamAttitude::Type ABotaniCrowdAIController::GetTeamAttitudeTowards(const AActor& Other) const
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

void ABotaniCrowdAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	/*if (BrainComponent->IsRunning())
	{
		BrainComponent->RestartLogic();
	}
	else
	{
		BrainComponent->StartLogic();
	}*/
}

void ABotaniCrowdAIController::OnUnPossess()
{
	Super::OnUnPossess();

	// BrainComponent->StopLogic("Dead");
}

void ABotaniCrowdAIController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamId, SharedParams);
}

void ABotaniCrowdAIController::OnRep_MyTeamId(const FGenericTeamId& OldTeamId)
{
	//TODO: Soll irgendwas passieren wenn sich das Team ändert?
}
