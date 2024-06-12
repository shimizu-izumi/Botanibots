#pragma once

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

class FCommonEquipmentEditorStyle final : public FSlateStyleSet
{
public:
	FCommonEquipmentEditorStyle() : FSlateStyleSet("CommonEquipmentEditorStyle")
	{
		const FString BaseDir = IPluginManager::Get().FindPlugin("CommonEquipment")->GetBaseDir();
		FSlateStyleSet::SetContentRoot(BaseDir / TEXT("Content"));
        
		FSlateStyleRegistry::RegisterSlateStyle(*this);
	}

	~FCommonEquipmentEditorStyle()
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*this);
	}
};