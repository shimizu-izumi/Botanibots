// Copyright © 2024 Botanibots Team. All rights reserved.


#include "BehaviorTree/Service/BTService_TryActivateAbilitiesByTag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AISystem.h"

UBTService_TryActivateAbilitiesByTag::UBTService_TryActivateAbilitiesByTag(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Try Activate Abilities By Tag";

	bNotifyBecomeRelevant = false;
	bNotifyCeaseRelevant = true;

	bUpdateBBOnFail = GET_AI_CONFIG_VAR(bClearBBEntryOnBTEQSFail);
}

void UBTService_TryActivateAbilitiesByTag::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	AActor* OwningPawn = OwnerComp.GetOwner();
	if (const AController* ControllerOwner = Cast<AController>(OwningPawn))
	{
		OwningPawn = ControllerOwner->GetPawn();
	}

	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPawn);
	if (!OwningPawn || !OwnerASC)
	{
		return;
	}

	OwnerASC->TryActivateAbilitiesByTag(AbilityTags);
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

FString UBTService_TryActivateAbilitiesByTag::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s:\n Try Activate Abilities by tag\n With Tags: %s"), *Super::GetStaticDescription(), *AbilityTags.ToString());
}
