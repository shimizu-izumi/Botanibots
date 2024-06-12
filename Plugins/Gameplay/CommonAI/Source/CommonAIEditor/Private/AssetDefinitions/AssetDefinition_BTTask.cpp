// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AssetDefinition_BTTask.h"

#include "CommonAIEditor.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "EditorStyle/CommonAIEditorStyle.h"

#define LOCTEXT_NAMESPACE "BTTask"

FText UAssetDefinition_BTTask::GetAssetDisplayName() const
{
	return LOCTEXT("AssetTypeActions_BTTask", "Behavior Tree Task");
}

FLinearColor UAssetDefinition_BTTask::GetAssetColor() const
{
	return FCommonAIEditorStyle::TypeColor;
}

TSoftClassPtr<> UAssetDefinition_BTTask::GetAssetClass() const
{
	return UBTTask_BlueprintBase::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_BTTask::GetAssetCategories() const
{
	static const auto Categories = {
		EAssetCategoryPaths::AI / NSLOCTEXT("ArtificialIntelligence", "ArtificialIntelligenceSubmenu_BehaviorTree", "Behavior Tree")
	};
		
	return Categories;
}

EAssetCommandResult UAssetDefinition_BTTask::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

	for (UBTTask_BlueprintBase* Definition : OpenArgs.LoadObjects<UBTTask_BlueprintBase>())
	{
		UCommonAIAssetEditor* AssetEditor = NewObject<UCommonAIAssetEditor>(AssetEditorSubsystem, NAME_None, RF_Transient);
		AssetEditor->SetObjectToEdit(Definition);
		AssetEditor->Initialize();
	}
	return EAssetCommandResult::Handled;
}
#undef LOCTEXT_NAMESPACE
