// Copyright © 2024 Botanibots Team. All rights reserved.


#include "CommonAIEditorStyle.h"

#include "Styling/SlateStyleRegistry.h"

TUniquePtr<FCommonAIEditorStyle> FCommonAIEditorStyle::Instance(nullptr);
FColor FCommonAIEditorStyle::TypeColor(63,126,255);

FCommonAIEditorStyle::~FCommonAIEditorStyle()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*this);
}

FCommonAIEditorStyle& FCommonAIEditorStyle::Get()
{
	if (!Instance.IsValid())
	{
		Instance = TUniquePtr<FCommonAIEditorStyle>(new FCommonAIEditorStyle);
	}
	return *(Instance.Get());
}

void FCommonAIEditorStyle::Shutdown()
{
	Instance.Reset();
}

FCommonAIEditorStyle::FCommonAIEditorStyle(): FSlateStyleSet("CommonAIEditorStyle")
{
	const FString PluginContentDir = FPaths::ProjectPluginsDir() / TEXT("CommonAI/Resources");
	FSlateStyleSet::SetContentRoot(PluginContentDir);
	

	// Set("ClassIcon.SmartObjectDefinition", new IMAGE_BRUSH_SVG("Icons/SmartObject_16", CoreStyleConstants::Icon16x16));
	// Set("ClassThumbnail.SmartObjectDefinition", new IMAGE_BRUSH_SVG("Icons/SmartObject_64", CoreStyleConstants::Icon64x64));
	// Set("ClassIcon.SmartObjectComponent", new IMAGE_BRUSH_SVG("Icons/SmartObject_16", CoreStyleConstants::Icon16x16));
	// Set("ClassThumbnail.SmartObjectComponent", new IMAGE_BRUSH_SVG("Icons/SmartObject_64", CoreStyleConstants::Icon64x64));

	FSlateStyleRegistry::RegisterSlateStyle(*this);
}
