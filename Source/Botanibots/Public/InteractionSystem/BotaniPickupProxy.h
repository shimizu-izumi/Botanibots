// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Pickup/ProxyActor/CommonPickupProxy.h"
#include "BotaniPickupProxy.generated.h"

class UTagStackComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UPointLightComponent;
class UNiagaraComponent;
class UCommonItemDefinition;

UCLASS(Abstract, DisplayName = "Pickup Proxy Actor")
class BOTANIBOTS_API ABotaniPickupProxy : public ACommonPickupProxy
{
	GENERATED_BODY()

public:
	ABotaniPickupProxy();

	//~Begin ABotaniPickupActor Contract
	virtual FGameplayTagContainer GetCreationContext() const;
	//~End ABotaniPickupActor Contract

	//~Begin ACommonPickupProxy Contract
	virtual void ResetPickupProxy() override;
	virtual void SetupPickupProxy(UCommonItemDefinition* NewItemDefinition) override;
	//~End ACommonPickupProxy Contract

#pragma region BlueprintFunctions

	/*
	 Get Creation Context
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pickup", meta = (DisplayName = "Get Creation Context"))
	FGameplayTagContainer K2_GetCreationContext() const { return GetCreationContext(); }

	/*
	 Force updates the current pickup
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "PickupProxy", meta = (DisplayName = "Force Update"))
	void ForceUpdate();

	/*
	 Unload
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "PickupProxy", meta = (DisplayName = "Unload"))
	void Unload();

	/*
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickupProxy")
	bool bEnablePickupMovementOnBeginPlay = false;

#pragma endregion

protected:
#pragma region Components
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNiagaraComponent> PickupGlowsSystem;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPointLightComponent> PickupLightComponent;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, Category = "Pickup|Visuals")
	TObjectPtr<UMaterialInterface> PickupGlowMaterial;
};
