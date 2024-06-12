// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BotaniWeaponItem.h"
#include "Interfaces/BotaniCombatInterface.h"
#include "BotaniRangedWeaponItem.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BOTANIBOTS_API ABotaniRangedWeaponItem : public ABotaniWeaponItem, public IBotaniCombatInterface
{
	GENERATED_BODY()

public:
	ABotaniRangedWeaponItem();

	//~ Begin ABotaniWeaponItem Interface
	virtual void FireWeapon(const FBotaniWeaponFirePayload& FirePayload) override;
	//~ End ABotaniWeaponItem Interface

	//~ Begin IBotaniCombatInterface
	virtual USkeletalMeshComponent* GetWeaponMesh_Implementation() const override;
	//~ End IBotaniCombatInterface

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName MuzzleFalloffSocketName = "MuzzleFalloff";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> MuzzleFalloffEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ScopeGlareEffect;

protected:
	//~Begin AActor Interface
	virtual void BeginPlay() override;
	//~End AActor Interface
};
