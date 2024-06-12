// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "BotaniUserWidget.generated.h"

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	Menu	UMETA(Tooltip = "Input is received by the UI only"),
	Game	UMETA(Tooltip = "Input is received by the Game only"),
	All		UMETA(Tooltip = "Input is received by UI and the Game"),
	Ignore	UMETA(Tooltip = "Constructin this Widget will not change the input mode")
};

/**
 * 
 */
UCLASS()
class BOTANIBOTS_API UBotaniUserWidget : public UCommonActivatableWidget 
{
	GENERATED_BODY()

public:
	//TODO: Widget Controller entfernen
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

protected:
	virtual void OnWidgetControllerSet(UObject* InWidgetController);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Widget Controller Set")
	void WidgetControllerSet_BP(UObject* NewWidgetController);
	
	virtual void NativeOnActivated() override;
	virtual void NativeConstruct() override;
	void ManageInputMode() const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	EInputMode InputMode = EInputMode::All;
};
