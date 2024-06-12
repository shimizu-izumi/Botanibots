// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BotaniInteractionWidget.generated.h"

class UTextBlock;
/**
 Interaction Widget
 */
UCLASS(Abstract)
class BOTANIBOTS_API UBotaniInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBotaniInteractionWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "Interaction")
	TObjectPtr<UTextBlock> InteractionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "Interaction")
	TObjectPtr<UTextBlock> InteractionObjectText;
};
