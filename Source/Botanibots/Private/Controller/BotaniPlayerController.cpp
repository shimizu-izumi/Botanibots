// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Controller/BotaniPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Component/BotaniAbilityComponent.h"
#include "GameplayTags/BotaniGameplayTags.h"
#include "Input/BotaniInputComponent.h"

ABotaniPlayerController::ABotaniPlayerController()
{
	bReplicates = true;
}

void ABotaniPlayerController::BeginPlay()
{
	Super::BeginPlay();
	checkf(InputMappingContext, TEXT("InputMappingContext is null in %s"), *GetName());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	FInputModeGameOnly InputModeData;
	InputModeData.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputModeData);
	bShowMouseCursor = false;
}

void ABotaniPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (GetAbilityComponent())
	{
		BotaniAbilityComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ABotaniPlayerController::PlayerMove(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.X);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.Y);
	}
}

void ABotaniPlayerController::PlayerLook(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(InputAxisVector.X);
		ControlledPawn->AddControllerPitchInput(InputAxisVector.Y);
	}
}

//OPT ********************************************************************************************************************
//TODO: Nur zum Testen, später entfernen
void ABotaniPlayerController::QuickSlot1Pressed()
{
	//UCommonEquipmentBlueprintLibrary::GetQuickBarComponent(GetPawn())->SetActiveSlotIndex(0);
}

void ABotaniPlayerController::QuickSlot2Pressed()
{
	//UCommonEquipmentBlueprintLibrary::GetQuickBarComponent(GetPawn())->SetActiveSlotIndex(1);
}

void ABotaniPlayerController::QuickSlot3Pressed()
{
	//UCommonEquipmentBlueprintLibrary::GetQuickBarComponent(GetPawn())->SetActiveSlotIndex(2);
}
//OPT ********************************************************************************************************************

void ABotaniPlayerController::AbilityInputStarted(FGameplayTag InputTag)
{
	if (const APawn* OwningPawn = GetPawn<APawn>())
	{
		if (GetAbilityComponent())
		{
			BotaniAbilityComponent->AbilityInputStarted(InputTag);
		}
		else
		{
			UE_LOG(LogBotaniInput, Error, TEXT("Unable to get AbilityComponent from Pawn: [%s] in: [%s]"), *OwningPawn->GetName(), *GetNameSafe(this));
		}
	}
}

void ABotaniPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (const APawn* OwningPawn = GetPawn<APawn>())
	{
		if (GetAbilityComponent())
		{
			BotaniAbilityComponent->AbilityInputPressed(InputTag);
		}
		else
		{
			UE_LOG(LogBotaniInput, Error, TEXT("Unable to get AbilityComponent from Pawn: [%s] in: [%s]"), *OwningPawn->GetName(), *GetNameSafe(this));
		}
	}
}

void ABotaniPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (const APawn* OwningPawn = GetPawn<APawn>())
	{
		if (GetAbilityComponent())
		{
			BotaniAbilityComponent->AbilityInputReleased(InputTag);
		}
		else
		{
			UE_LOG(LogBotaniInput, Error, TEXT("Unable to get AbilityComponent from Pawn: [%s] in: [%s]"), *OwningPawn->GetName(), *GetNameSafe(this));
		}
	}
}

UBotaniAbilityComponent* ABotaniPlayerController::GetAbilityComponent()
{
	if (BotaniAbilityComponent == nullptr)
	{
		BotaniAbilityComponent = Cast<UBotaniAbilityComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}

	return BotaniAbilityComponent;
}

void ABotaniPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UBotaniInputComponent* EnhancedInputComponent = CastChecked<UBotaniInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputStarted, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);

		EnhancedInputComponent->BindNativeAction(InputConfig, BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_Move, ETriggerEvent::Triggered, this, &ThisClass::PlayerMove, true);
		EnhancedInputComponent->BindNativeAction(InputConfig, BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_Look, ETriggerEvent::Triggered, this, &ThisClass::PlayerLook, true);

		//OPT ********************************************************************************************************************
		//TODO: Nur zum Testen, später entfernen
		EnhancedInputComponent->BindNativeAction(InputConfig, BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_QuickAccess1, ETriggerEvent::Started, this, &ThisClass::QuickSlot1Pressed, true);
		EnhancedInputComponent->BindNativeAction(InputConfig, BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_QuickAccess2, ETriggerEvent::Started, this, &ThisClass::QuickSlot2Pressed, true);
		EnhancedInputComponent->BindNativeAction(InputConfig, BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_QuickAccess3, ETriggerEvent::Started, this, &ThisClass::QuickSlot3Pressed, true);
		//OPT ********************************************************************************************************************
	}
}

void ABotaniPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
