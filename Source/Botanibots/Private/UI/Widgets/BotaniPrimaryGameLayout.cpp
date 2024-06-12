// Copyright © 2024 Botanibots Team. All rights reserved.


#include "UI/Widgets/BotaniPrimaryGameLayout.h"

#include "CommonActivatableWidget.h"
#include "Engine/GameInstance.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UBotaniPrimaryGameLayout::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	if (IsValid(InWidgetController))
	{
		WidgetControllerSet(InWidgetController);	
	}
}

void UBotaniPrimaryGameLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	if (!IsDesignTime())
	{
		LayerWidget->SetTransitionDuration(0.f);
		Layers.Add(LayerTag, LayerWidget);
	}
}

UCommonActivatableWidgetContainerBase* UBotaniPrimaryGameLayout::GetLayerWidget(FGameplayTag LayerTag)
{
	return Layers.FindRef(LayerTag);
}

void UBotaniPrimaryGameLayout::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	for (const auto& LayerKVP : Layers)
	{
		LayerKVP.Value->RemoveWidget(*ActivatableWidget);
	}
}

UCommonActivatableWidget* UBotaniPrimaryGameLayout::PushWidgetToLayerStack_BP(const FGameplayTag LayerTag, const TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass)
{
	return PushWidgetToLayerStack<UCommonActivatableWidget>(LayerTag, ActivatableWidgetClass);
}

