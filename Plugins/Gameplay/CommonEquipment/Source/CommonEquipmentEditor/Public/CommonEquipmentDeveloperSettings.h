// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Inventory/Definition/CommonItemDefinition.h"
#include "CommonEquipmentDeveloperSettings.generated.h"

USTRUCT(BlueprintType)
struct FCommonEquipmentDefinitionStyle
{
	GENERATED_BODY()

public:

	FCommonEquipmentDefinitionStyle()
	{
	}
	
	TSubclassOf<UCommonItemDefinition> ItemClass = UCommonItemDefinition::StaticClass();

	UPROPERTY(EditAnywhere, Category = "Style")
	FLinearColor Color = FColor::FromHex("26601e");

	UPROPERTY(EditAnywhere, Category = "Style")
	bool bSRGB = true;

	UPROPERTY(EditAnywhere, Category = "Style")
	FLinearColor DefaultBackgroundColor = FColor::FromHex("1a1a1a");

	UPROPERTY(EditAnywhere, Category = "Style")
	bool bDefaultBackgroundSRGB = true;

	UPROPERTY(EditAnywhere, Category = "Style")
	TObjectPtr<UTexture2D> DefaultIcon = nullptr;
};

/**
 * 
 */
UCLASS(config=EditorPerProjectUserSettings, DisplayName="Common Equipment")
class COMMONEQUIPMENTEDITOR_API UCommonEquipmentDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//~ Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override;
	//~ End UDeveloperSettings Interface

#if WITH_EDITORONLY_DATA
	UPROPERTY(Config, EditAnywhere, Category = "Common Equipment|Style", meta = (ForceInlineRow))
	FCommonEquipmentDefinitionStyle ItemStyle;
#endif
};
