// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Tools/UAssetEditor.h"
#include "CommonAIEditor.generated.h"

class IAssetTools;
class IAssetTypeActions;

class ICommonAIEditorModule : public IModuleInterface
{
public:

    /**
     * Singleton-like access to this module's interface.  This is just for convenience!
     * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
     *
     * @return Returns singleton instance, loading the module on demand if needed
     */
    static inline ICommonAIEditorModule& Get()
    {
        return FModuleManager::LoadModuleChecked<ICommonAIEditorModule>("CommonAIEditor");
    }

    /**
     * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
     *
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("CommonAIEditor");
    }
};

UCLASS(Transient)
class UCommonAIAssetEditor : public UAssetEditor
{
	GENERATED_BODY()
public:
	void SetObjectToEdit(UObject* InObject);

protected:
	virtual void GetObjectsToEdit(TArray<UObject*>& OutObjectsToEdit) override;
	virtual TSharedPtr<FBaseAssetToolkit> CreateToolkit() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UObject> ObjectToEdit;
};
