// Copyright © 2024 Botanibots Team. All rights reserved.


#include "UI/Widgets/BotaniTaggedWidget.h"

UBotaniTaggedWidget::UBotaniTaggedWidget()
{
}

void UBotaniTaggedWidget::SetVisibility(ESlateVisibility InVisibility)
{
#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		Super::SetVisibility(InVisibility);
		return;
	}
#endif
	
	bForceVisibility = ConvertSerializedVisibilityToRuntime(InVisibility).IsVisible();
	if (bForceVisibility)
	{
		ShownVisibility = InVisibility;
	}
	else
	{
		HiddenVisibility = InVisibility;
	}

	//@TODO: Pawn->HasAnyTags(HiddenByTags);
	const bool bHasHiddenTags = false;

	// Actually apply the visibility
	const ESlateVisibility DesiredVisibility = (bForceVisibility && !bHasHiddenTags) ? ShownVisibility : HiddenVisibility;
	if (GetVisibility() != DesiredVisibility)
	{
		Super::SetVisibility(DesiredVisibility);
	}
}

void UBotaniTaggedWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!IsDesignTime())
	{
		//TODO: Listen for tag changes
		SetVisibility(GetVisibility());
	}
}

void UBotaniTaggedWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (!IsDesignTime())
	{
		//TODO: Stop listening for tag changes
	}
}

void UBotaniTaggedWidget::OnWatchedTagsChanged()
{
	//@TODO: Pawn->HasAnyTags(HiddenByTags);
	const bool bHasHiddenTags = false;

	const ESlateVisibility DesiredVisibility = (bForceVisibility && !bHasHiddenTags) ? ShownVisibility : HiddenVisibility;
	if (GetVisibility() != DesiredVisibility)
	{
		Super::SetVisibility(DesiredVisibility);
	}
}
