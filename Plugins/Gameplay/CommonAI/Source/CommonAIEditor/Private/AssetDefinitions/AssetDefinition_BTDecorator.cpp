// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AssetDefinition_BTDecorator.h"

#include "CommonAIEditor.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "EditorStyle/CommonAIEditorStyle.h"
#define LOCTEXT_NAMESPACE "BTDecorator"


FText UAssetDefinition_BTDecorator::GetAssetDisplayName() const
{
	return LOCTEXT("AssetTypeActions_BTDecorator", "Behavior Tree Decorator");
}

FLinearColor UAssetDefinition_BTDecorator::GetAssetColor() const
{
	return FCommonAIEditorStyle::TypeColor;
}

TSoftClassPtr<> UAssetDefinition_BTDecorator::GetAssetClass() const
{
	return UBTDecorator_BlueprintBase::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_BTDecorator::GetAssetCategories() const
{
	static const auto Categories = {
		EAssetCategoryPaths::AI / NSLOCTEXT("ArtificialIntelligence", "ArtificialIntelligenceSubmenu_BehaviorTree", "Behavior Tree")
	};
		
	return Categories;
}

EAssetCommandResult UAssetDefinition_BTDecorator::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

	for (UBTDecorator_BlueprintBase* Definition : OpenArgs.LoadObjects<UBTDecorator_BlueprintBase>())
	{
		UCommonAIAssetEditor* AssetEditor = NewObject<UCommonAIAssetEditor>(AssetEditorSubsystem, NAME_None, RF_Transient);
		AssetEditor->SetObjectToEdit(Definition);
		AssetEditor->Initialize();
	}
	return EAssetCommandResult::Handled;
}

#undef LOCTEXT_NAMESPACE