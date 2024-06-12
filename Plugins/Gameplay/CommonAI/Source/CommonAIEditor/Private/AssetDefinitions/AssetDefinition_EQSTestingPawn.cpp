// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AssetDefinition_EQSTestingPawn.h"

#include "CommonAIEditor.h"
#include "EditorStyle/CommonAIEditorStyle.h"
#include "EnvironmentQuery/EQSTestingPawn.h"

#define LOCTEXT_NAMESPACE "EQSTestingPawn"


FText UAssetDefinition_EQSTestingPawn::GetAssetDisplayName() const
{
	return LOCTEXT("AssetTypeActions_EQSTestingPawn", "EnvQuery Testing Pawn");
}

FLinearColor UAssetDefinition_EQSTestingPawn::GetAssetColor() const
{
	return FCommonAIEditorStyle::TypeColor;
}

TSoftClassPtr<> UAssetDefinition_EQSTestingPawn::GetAssetClass() const
{
	return AEQSTestingPawn::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_EQSTestingPawn::GetAssetCategories() const
{
	static const auto Categories = {
		EAssetCategoryPaths::AI / NSLOCTEXT("ArtificialIntelligence", "ArtificialIntelligenceSubmenu_EQS", "Environment Query"),
	};
		
	return Categories;
}

EAssetCommandResult UAssetDefinition_EQSTestingPawn::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

	for (AEQSTestingPawn* Definition : OpenArgs.LoadObjects<AEQSTestingPawn>())
	{
		UCommonAIAssetEditor* AssetEditor = NewObject<UCommonAIAssetEditor>(AssetEditorSubsystem, NAME_None, RF_Transient);
		AssetEditor->SetObjectToEdit(Definition);
		AssetEditor->Initialize();
	}
	return EAssetCommandResult::Handled;
}

#undef LOCTEXT_NAMESPACE