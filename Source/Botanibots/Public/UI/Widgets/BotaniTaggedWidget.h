// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BotaniUserWidget.h"
#include "GameplayTagContainer.h"
#include "BotaniTaggedWidget.generated.h"

/**
Botani Tagged Widget
Widget which can be hidden or shown via tags on the owning player
 */
UCLASS()
class BOTANIBOTS_API UBotaniTaggedWidget : public UBotaniUserWidget
{
	GENERATED_BODY()

public:
	UBotaniTaggedWidget();

	//~begin UWidget interface
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	//~end UWidget interface

	//~begin UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~end UUserWidget interface

protected:
	/*
	Hidden by Tags
	If the owning player has any of these tags, this widget will be hidden
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	FGameplayTagContainer HiddenByTags;

	/*
	Shown Visibility
	The visibility to use when this widget is set to be shown
	 */
	UPROPERTY(EditAnywhere, Category = "HUD")
	ESlateVisibility ShownVisibility = ESlateVisibility::Visible;

	/*
	Hidden Visibility
	The visibility to use when this widget is set to be hidden
	 */
	UPROPERTY(EditAnywhere, Category = "HUD")
	ESlateVisibility HiddenVisibility = ESlateVisibility::Collapsed;

	/*
	Force Visibility
	Should this widget be forced to be visible, ignoring tags
	 */
	bool bForceVisibility = true;

private:
	void OnWatchedTagsChanged();
};

