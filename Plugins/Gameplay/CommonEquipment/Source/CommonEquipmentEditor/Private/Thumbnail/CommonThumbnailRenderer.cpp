// Copyright © 2024 Botanibots Team. All rights reserved.


#include "CommonThumbnailRenderer.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "CommonEquipmentDeveloperSettings.h"
#include "Inventory/Definition/CommonItemDefinition.h"
#include "ImageUtils.h"

class UCommonItemDefinition;

UCommonThumbnailRenderer::UCommonThumbnailRenderer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCommonThumbnailRenderer::GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const
{
	if (const UCommonItemDefinition* DataAsset = Cast<UCommonItemDefinition>(Object))
	{
		if (const UTexture2D* Texture = DataAsset->ItemIcon.LoadSynchronous())
		{
			OutWidth = FMath::TruncToInt(Zoom * Texture->GetSurfaceWidth());
			OutHeight = FMath::TruncToInt(Zoom * Texture->GetSurfaceHeight());
		}
	}
	Super::GetThumbnailSize(Object, Zoom, OutWidth, OutHeight);
}

void UCommonThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height,
	FRenderTarget* Viewport, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	if (const UCommonItemDefinition* DataAsset = Cast<UCommonItemDefinition>(Object))
	{
		if (UTexture2D* Texture2D = DataAsset->ItemIcon.LoadSynchronous())
		{
			const bool bUseTranslucentBlend = Texture2D && Texture2D->HasAlphaChannel() && ((Texture2D->LODGroup == TEXTUREGROUP_UI) || (Texture2D->LODGroup == TEXTUREGROUP_Pixels2D));
			TRefCountPtr<FBatchedElementParameters> BatchedElementParameters;
			if (bUseTranslucentBlend)
			{
				// If using alpha, draw a checkerboard underneath first.
				const int32 CheckerDensity = 8;
				auto* Checker = FImageUtils::CreateCheckerboardTexture(FColor::FromHex("808080"), FColor::FromHex("404040"), 8);
				Canvas->DrawTile(
					0.0f, 0.0f, Width, Height,
					0.0f, 0.0f, CheckerDensity, CheckerDensity,
					FLinearColor::White, Checker->GetResource());
			}
			FCanvasTileItem CanvasTile(FVector2D(X, Y), Texture2D->GetResource(), FVector2D(Width, Height), FLinearColor::White);
			CanvasTile.BlendMode = bUseTranslucentBlend ? SE_BLEND_Translucent : SE_BLEND_Opaque;
			CanvasTile.BatchedElementParameters = BatchedElementParameters;
			CanvasTile.Draw(Canvas);
			if (Texture2D && Texture2D->IsCurrentlyVirtualTextured())
			{
				auto VTChars = TEXT("VT");
				int32 VTWidth = 0;
				int32 VTHeight = 0;
				StringSize(GEngine->GetLargeFont(), VTWidth, VTHeight, VTChars);
				float PaddingX = Width / 128.0f;
				float PaddingY = Height / 128.0f;
				float ScaleX = Width / 64.0f;
				float ScaleY = Height / 64.0f;
				FCanvasTextItem TextItem(FVector2D(Width - PaddingX - VTWidth * ScaleX, Height - PaddingY - VTHeight * ScaleY), FText::FromString(VTChars), GEngine->GetLargeFont(), FLinearColor::White);
				TextItem.EnableShadow(FLinearColor::Black);
				TextItem.Scale = FVector2D(ScaleX, ScaleY);
				TextItem.Draw(Canvas);
			}
			return;
		}

		/* If we don't have a texture, draw a checkerboard with the default data asset icon
		FColor DefaultColor = FColor::FromHex("1a1a1a");
		UObject* Loaded = LoadObject<UObject>(nullptr, TEXT("/CommonEquipment/Icons/DataAsset_64.DataAsset_64"));
		UTexture2D* DefaultIcon = Cast<UTexture2D>(Loaded);

		const UCommonEquipmentDeveloperSettings* DevSettings = GetDefault<UCommonEquipmentDeveloperSettings>();
		if (DevSettings)
		{
			if (DevSettings->ItemStyle.DefaultIcon)
			{
				DefaultIcon = DevSettings->ItemStyle.DefaultIcon;
			}

			DefaultColor = DevSettings->ItemStyle.DefaultBackgroundColor.ToFColor(DevSettings->ItemStyle.bDefaultBackgroundSRGB);
		}
		
		UTexture2D* Checker = FImageUtils::CreateCheckerboardTexture(DefaultColor, DefaultColor,1);


		const float SizeMult = 0.9f;
		const float Size = Height * SizeMult;
		const float Padding = (Height - (Height * SizeMult)) / 2.f;
		
		Canvas->DrawTile(0.f, 0.f, Width, Height, 0.f, 0.f, 1.f, 1.f, FLinearColor::White, Checker->GetResource());
		Canvas->DrawTile(Padding, Padding, Size, Size, 0.f, 0.f, 1.f, 1.f, FLinearColor::White, DefaultIcon->GetResource());
		return;*/
	}
	Super::Draw(Object, X, Y, Width, Height, Viewport, Canvas, bAdditionalViewFamily);
}

bool UCommonThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
	if (Cast<UCommonItemDefinition>(Object))
	{
		return true;
	}
	
	return Super::CanVisualizeAsset(Object);
}

UTexture2D* UCommonThumbnailRenderer::GetIconFromDataAsset(UDataAsset* Class)
{
	if (const UCommonItemDefinition* TestAsset = Cast<UCommonItemDefinition>(Class))
	{
		return TestAsset->ItemIcon.LoadSynchronous();
	}
	return nullptr;
}
