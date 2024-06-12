// Copyright © 2024 Botanibots Team. All rights reserved.


#include "CommonItemActions.h"

#include "CommonEquipmentDeveloperSettings.h"
#include "Inventory/Definition/CommonItemDefinition.h"
#define LOCTEXT_NAMESPACE "AssetTypeActions"

FCommonItemActions::FCommonItemActions(const TSharedRef<ISlateStyle>& InStyle)
	: Style(InStyle)
{
}

FColor FCommonItemActions::GetTypeColor() const
{
	const UCommonEquipmentDeveloperSettings* DevSettings = GetDefault<UCommonEquipmentDeveloperSettings>();
	if (!DevSettings)
	{
		return FColor::FromHex("26601e");	
	}

	return DevSettings->ItemStyle.Color.ToFColor(DevSettings->ItemStyle.bSRGB);
}

bool FCommonItemActions::CanFilter()
{
	return FAssetTypeActions_DataAsset::CanFilter();
}

UClass* FCommonItemActions::GetSupportedClass() const
{
	return UCommonItemDefinition::StaticClass();
}

FText FCommonItemActions::GetDisplayNameFromAssetData(const FAssetData& AssetData) const
{
	TArray<FString> FilterWords = {TEXT("Botani"), TEXT("Common")};
	FString Left, Right, Result;
	AssetData.AssetClassPath.ToString().Split(".", &Left, &Right);
	
	for (const FString& FilterWord : FilterWords)
	{
		Right = Right.Replace(*FilterWord, TEXT(""));
	}

	for (int32 i = 0; i < Right.Len(); ++i)
	{
		if (FChar::IsUpper(Right[i]) && i != 0)
		{
			Result.AppendChar(' ');
		}
		Result.AppendChar(Right[i]);
	}
	
	return FText::FromString(Result);
}

FText FCommonItemActions::GetAssetDescription(const FAssetData& AssetData) const
{
	return FText::FromString(TEXT("Item Definition for modular equipment."));
}
#undef LOCTEXT_NAMESPACE