// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ModularPlayerController.h"
#include "BotaniPlayerController.generated.h"

class UBotaniAbilityComponent;
class UBotaniInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class BOTANIBOTS_API ABotaniPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ABotaniPlayerController();

protected:
	//~ begin APlayerController interface
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~ end APlayerController interface
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

private:
	/** Input Setup */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UBotaniInputConfig> InputConfig;

	/** Input Functions */
	void PlayerMove(const FInputActionValue& Value);
	void PlayerLook(const FInputActionValue& Value);

	void QuickSlot1Pressed();
	void QuickSlot2Pressed();
	void QuickSlot3Pressed();

	void AbilityInputStarted(FGameplayTag InputTag);
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UBotaniAbilityComponent> BotaniAbilityComponent;

	UBotaniAbilityComponent* GetAbilityComponent();
};
