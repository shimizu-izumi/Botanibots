#pragma once

#include "CoreMinimal.h"
#include "IAssetTools.h"
#include "Modules/ModuleManager.h"
#include "Tools/UAssetEditor.h"
#include "CommonEquipmentEditor.generated.h"

class FCommonEquipmentEditorModule final : public IModuleInterface
{
public:
    //~ Begin IModuleInterface
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    virtual bool SupportsDynamicReloading() override;
    //~ End IModuleInterface

    static FSlateImageBrush* DefaultBrush;
    static TSharedPtr<FSlateStyleSet> StyleSet;

protected:
    void RegisterAssetTools();
    void UnregisterAssetTools();
    void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

private:
    TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
};


UCLASS(Transient)
class COMMONEQUIPMENTEDITOR_API UCommonEquipmentAssetEditor : public UAssetEditor
{
    GENERATED_BODY()

public:
    void SetObjectToEdit(UObject* InObject);

protected:
    virtual void GetObjectsToEdit(TArray<UObject*>& InObjectsToEdit) override;

private:
    UPROPERTY(Transient)
    TObjectPtr<UObject> ObjectToEdit;
};