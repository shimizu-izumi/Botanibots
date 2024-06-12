// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Styling/SlateStyle.h"

class FCommonAIEditorStyle final : public FSlateStyleSet
{
public:
	virtual ~FCommonAIEditorStyle() override;

	static FCommonAIEditorStyle& Get();
	static void Shutdown();

	static FColor TypeColor;

private:
	FCommonAIEditorStyle();

	static TUniquePtr<FCommonAIEditorStyle> Instance;
};