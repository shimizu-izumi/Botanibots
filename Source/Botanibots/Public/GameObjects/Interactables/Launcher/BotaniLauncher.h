// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "BotaniLauncher.generated.h"

UCLASS(Abstract)
class BOTANIBOTS_API ABotaniLauncher : public AActor
{
	GENERATED_BODY()

public:
	ABotaniLauncher();
	
	/** Launcher Settings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotaniLauncher")
	float UpOverride = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotaniLauncher")
	float MasterMultiplier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotaniLauncher")
	bool bDoNotTriggerOnFallEntry = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotaniLauncher")
	float MinimumVerticalVelocityAllowed = -1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotaniLauncher|Target")
	bool bUseTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotaniLauncher|Target", meta = (ExposeOnSpawn = true, MakeEditWidget = true, EditCondition = "bUseTarget"))
	FVector TargetBounds = FVector(0.f, 0.f, 0.f);

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "BotaniLauncher|Target", meta = (EditCondition = "bUseTarget"))
	bool bLockTargetBounds = false;
#endif
	
	/** Cosmetics */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BotaniLauncher|Cosmetics")
	FGameplayTag LauncherActivateCueTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotaniLauncher|Cosmetics")
	FLinearColor LauncherColor = FLinearColor::Blue;

	/** Components */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USphereComponent> SphereCollisionComponent;

protected:
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif
	virtual void BeginPlay() override;
	virtual bool TargetOverlapped(ACharacter* InOverlappedCharacter) {return false; }
	
private:
	/** Functions */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_ActivationVFX();

	/** Variables */
	TObjectPtr<ACharacter> OverlappedCharacter;
};
