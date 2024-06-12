// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AssetDefinition_BTService.h"

#include "CommonAIEditor.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "EditorStyle/CommonAIEditorStyle.h"

#define LOCTEXT_NAMESPACE "BTService"

FText UAssetDefinition_BTService::GetAssetDisplayName() const
{
	return LOCTEXT("AssetTypeActions_BTService", "Behavior Tree Service");
}

FLinearColor UAssetDefinition_BTService::GetAssetColor() const
{
	return FCommonAIEditorStyle::TypeColor;
}

TSoftClassPtr<> UAssetDefinition_BTService::GetAssetClass() const
{
	return UBTService_BlueprintBase::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_BTService::GetAssetCategories() const
{
	static const auto Categories = {
		EAssetCategoryPaths::AI / NSLOCTEXT("ArtificialIntelligence", "ArtificialIntelligenceSubmenu_BehaviorTree", "Behavior Tree")
	};
		
	return Categories;
}

EAssetCommandResult UAssetDefinition_BTService::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

	for (UBTService_BlueprintBase* Definition : OpenArgs.LoadObjects<UBTService_BlueprintBase>())
	{
		UCommonAIAssetEditor* AssetEditor = NewObject<UCommonAIAssetEditor>(AssetEditorSubsystem, NAME_None, RF_Transient);
		AssetEditor->SetObjectToEdit(Definition);
		AssetEditor->Initialize();
	}
	return EAssetCommandResult::Handled;
}
#undef LOCTEXT_NAMESPACE