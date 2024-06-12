// Copyright © 2024 Botanibots Team. All rights reserved.


#include "UI/Widgets/BotaniUserWidget.h"

void UBotaniUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	if (IsValid(InWidgetController))
		OnWidgetControllerSet(WidgetController);
}

void UBotaniUserWidget::OnWidgetControllerSet(UObject* InWidgetController)
{
	WidgetControllerSet_BP(InWidgetController);
}

void UBotaniUserWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	ManageInputMode();
}

void UBotaniUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ManageInputMode();
}

void UBotaniUserWidget::ManageInputMode() const
{
	switch (InputMode)
	{
	case EInputMode::Game:
		{
			FInputModeGameOnly WidgetInputMode;
			WidgetInputMode.SetConsumeCaptureMouseDown(false);
		
			GetOwningPlayer()->SetShowMouseCursor(false);
			GetOwningPlayer()->SetInputMode(WidgetInputMode);
			break;	
		}
	case EInputMode::All:
		{
			FInputModeGameAndUI WidgetInputMode;
			WidgetInputMode.SetHideCursorDuringCapture(true);
			WidgetInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			
			GetOwningPlayer()->SetShowMouseCursor(true);
			GetOwningPlayer()->SetInputMode(WidgetInputMode);
			break;	
		}
	case EInputMode::Menu:
		{
			FInputModeUIOnly WidgetInputMode;
			WidgetInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		
			GetOwningPlayer()->SetShowMouseCursor(true);
			GetOwningPlayer()->SetInputMode(WidgetInputMode);
			break;	
		}
	case EInputMode::Ignore:
		break;
	}
}
