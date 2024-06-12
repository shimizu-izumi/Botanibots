#include "CommonEquipmentEditor.h"

#include "AssetToolsModule.h"
#include "AssetActions/CommonItemActions.h"
#include "Inventory/Definition/CommonItemDefinition.h"
#include "Style/CommonEquipmentEditorStyle.h"
#include "Thumbnail/CommonThumbnailRenderer.h"

#define LOCTEXT_NAMESPACE "FCommonEquipmentEditorModule"
TSharedPtr<FSlateStyleSet> FCommonEquipmentEditorModule::StyleSet;
FSlateImageBrush* FCommonEquipmentEditorModule::DefaultBrush;

void FCommonEquipmentEditorModule::StartupModule()
{
    StyleSet = MakeShareable(new FCommonEquipmentEditorStyle());
	RegisterAssetTools();

	UThumbnailManager::Get().UnregisterCustomRenderer(UCommonItemDefinition::StaticClass());
	UThumbnailManager::Get().RegisterCustomRenderer(UCommonItemDefinition::StaticClass(), UCommonThumbnailRenderer::StaticClass());
}

void FCommonEquipmentEditorModule::ShutdownModule()
{
    UnregisterAssetTools();
}

bool FCommonEquipmentEditorModule::SupportsDynamicReloading()
{
	return true;
}

void FCommonEquipmentEditorModule::RegisterAssetTools()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FCommonItemActions(StyleSet.ToSharedRef())));
}

void FCommonEquipmentEditorModule::UnregisterAssetTools()
{
	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
	if (AssetToolsModule != nullptr)
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();
		for (auto Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}
}

void FCommonEquipmentEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}

void UCommonEquipmentAssetEditor::SetObjectToEdit(UObject* InObject)
{
	ObjectToEdit = InObject;
}

void UCommonEquipmentAssetEditor::GetObjectsToEdit(TArray<UObject*>& InObjectsToEdit)
{
	InObjectsToEdit.Add(ObjectToEdit);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCommonEquipmentEditorModule, CommonEquipmentEditor)