// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_TryActivateAbilitiesByTag.generated.h"

/**
 * 
 */
UCLASS()
class COMMONAI_API UBTService_TryActivateAbilitiesByTag : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTagContainer AbilityTags;

	UPROPERTY(EditAnywhere, Category = "Ability")
	bool bUpdateBBOnFail = false;

	UBTService_TryActivateAbilitiesByTag(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;
	
};
