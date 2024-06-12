// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/ClassThumbnailRenderer.h"
#include "CommonThumbnailRenderer.generated.h"

/**
 * 
 */
UCLASS()
class COMMONEQUIPMENTEDITOR_API UCommonThumbnailRenderer : public UClassThumbnailRenderer
{
	GENERATED_BODY()

public:
	UCommonThumbnailRenderer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~ Begin UThumbnailRenderer Interface
	virtual void GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const override;
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas, bool bAdditionalViewFamily) override;
	virtual bool CanVisualizeAsset(UObject* Object) override;
	//~ End UThumbnailRenderer Interface

	static UTexture2D* GetIconFromDataAsset(UDataAsset* Class);
};
