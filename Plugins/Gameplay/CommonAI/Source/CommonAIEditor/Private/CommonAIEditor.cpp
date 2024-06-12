// Copyright © 2024 Botanibots Team. All rights reserved.

#include "CommonAIEditor.h"

#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"
#include "EditorStyle/CommonAIEditorStyle.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"

#define LOCTEXT_NAMESPACE "CommonAI"

namespace UE::AI
{
	FName EditorBuildType(TEXT("CommonAICollections"));
}

class FCommonAIEditorModule : public ICommonAIEditorModule
{
protected:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void RegisterComponentVisualizer(FName ComponentClassName, TSharedPtr<FComponentVisualizer> Visualizer);

private:
	TArray<FName> RegisteredComponentClassNames;
};

void FCommonAIEditorModule::StartupModule()
{
	FCommonAIEditorStyle::Get();

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	int32 MyCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("CommonAI")), LOCTEXT("CommonAICategory", "CommonAI"));
}

void FCommonAIEditorModule::ShutdownModule()
{
	
	// Unregister all component visualizers
	if (GEngine)
	{
		for (const FName ClassName : RegisteredComponentClassNames)
		{
			GUnrealEd->UnregisterComponentVisualizer(ClassName);
		}
	}

	FCommonAIEditorStyle::Shutdown();
}

void FCommonAIEditorModule::RegisterComponentVisualizer(const FName ComponentClassName, const TSharedPtr<FComponentVisualizer> Visualizer)
{
	if (GUnrealEd != nullptr && Visualizer.IsValid())
	{
		GUnrealEd->RegisterComponentVisualizer(ComponentClassName, Visualizer);
		Visualizer->OnRegister();

		RegisteredComponentClassNames.Add(ComponentClassName);
	}
}

IMPLEMENT_MODULE(FCommonAIEditorModule, CommonAIEditor)

#undef LOCTEXT_NAMESPACE

void UCommonAIAssetEditor::SetObjectToEdit(UObject* InObject)
{
	ObjectToEdit = InObject;
}

void UCommonAIAssetEditor::GetObjectsToEdit(TArray<UObject*>& OutObjectsToEdit)
{
	OutObjectsToEdit.Add(ObjectToEdit);
}

TSharedPtr<FBaseAssetToolkit> UCommonAIAssetEditor::CreateToolkit()
{
	return Super::CreateToolkit();
}
