// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GenericTeamAgentBlueprintInterface.generated.h"


//TODO: Funktioniert noch nicht :(
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGenericTeamAgentBlueprintInterface : public UGenericTeamAgentInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class IGenericTeamAgentBlueprintInterface : public IGenericTeamAgentInterface
{
	GENERATED_IINTERFACE_BODY()
	
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override {SetGenericTeamIdBP(TeamID); }
	virtual FGenericTeamId GetGenericTeamId() const override {return GetGenericTeamIdBP(); }
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override {return GetTeamAttitudeTowardsBP(&Other); }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Generic Team Interface", meta = (DisplayName = "Set Generic Team ID"))
	void SetGenericTeamIdBP(const FGenericTeamId& TeamID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Generic Team Interface", meta = (DisplayName = "Get Generic Team ID"))
	FGenericTeamId GetGenericTeamIdBP() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Generic Team Interface", meta = (DisplayName = "Get Team Attitude Towards"))
	ETeamAttitude::Type GetTeamAttitudeTowardsBP(const AActor* Other) const;
};

inline UGenericTeamAgentBlueprintInterface::UGenericTeamAgentBlueprintInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Constructor logic here
}
