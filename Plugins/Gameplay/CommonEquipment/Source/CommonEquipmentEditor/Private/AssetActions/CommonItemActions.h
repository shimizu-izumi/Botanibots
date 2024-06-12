// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions/AssetTypeActions_DataAsset.h"

class FCommonItemActions final : public FAssetTypeActions_DataAsset
{
public:
	FCommonItemActions(const TSharedRef<ISlateStyle>& InStyle);

	//~ Begin IAssetTypeActions Interface
	virtual FColor GetTypeColor() const override;
	virtual bool CanFilter() override;
	virtual UClass* GetSupportedClass() const override;
	virtual FText GetDisplayNameFromAssetData(const FAssetData& AssetData) const override;
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override;
	//~ End IAssetTypeActions Interface

private:
	TSharedRef<ISlateStyle> Style;
};