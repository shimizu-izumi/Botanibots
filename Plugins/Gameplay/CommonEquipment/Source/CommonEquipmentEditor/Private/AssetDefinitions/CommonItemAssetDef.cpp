// Copyright © 2024 Botanibots Team. All rights reserved.


#include "CommonItemAssetDef.h"

#include "CommonEquipmentDeveloperSettings.h"
#include "Inventory/Definition/CommonItemDefinition.h"
#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText UCommonItemAssetDef::GetAssetDisplayName() const
{
	return LOCTEXT("AssetTypeActions_ItemDefinition", "Item Definition");
}

FLinearColor UCommonItemAssetDef::GetAssetColor() const
{
	return Super::GetAssetColor();
}

TSoftClassPtr<UObject> UCommonItemAssetDef::GetAssetClass() const
{
	return UCommonItemDefinition::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UCommonItemAssetDef::GetAssetCategories() const
{
	return Super::GetAssetCategories();
}

EAssetCommandResult UCommonItemAssetDef::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	return Super::OpenAssets(OpenArgs);
}
#undef LOCTEXT_NAMESPACE