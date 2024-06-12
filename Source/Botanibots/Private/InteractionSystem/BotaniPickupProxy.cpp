// Copyright © 2024 Botanibots Team. All rights reserved.


#include "InteractionSystem/BotaniPickupProxy.h"

#include "Components/SphereComponent.h"
#include "GameplayTagStacks/Components/TagStackComponent.h"
#include "NiagaraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/WidgetComponent.h"
#include "EquipmentSystem/Inventory/Definition/BotaniItemDefinitionData.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/BotaniGameplayTags.h"
#include "Inventory/Definition/CommonItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "Singletons/BotaniGameInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BotaniPickupProxy)

ABotaniPickupProxy::ABotaniPickupProxy()
{
	PickupGlowsSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Pickup Glows System"));
	if (PickupGlowsSystem)
	{
		PickupGlowsSystem->SetupAttachment(PickupHitboxComponent);
		PickupGlowsSystem->SetAutoActivate(true);
	}

	PickupLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	if (PickupLightComponent)
	{
		PickupLightComponent->SetupAttachment(PickupHitboxComponent);
		PickupLightComponent->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
		PickupLightComponent->SetIntensity(1000.f);
		PickupLightComponent->SetAttenuationRadius(125.f);
		PickupLightComponent->SetSourceRadius(80.f);
		PickupLightComponent->SetCastShadows(false);
	}
}

void ABotaniPickupProxy::ForceUpdate()
{
	SetupPickupProxy(CachedItemDefinition);
}

void ABotaniPickupProxy::Unload()
{
	ResetPickupProxy();
}

FGameplayTagContainer ABotaniPickupProxy::GetCreationContext() const
{
	FGameplayTagContainer CreationContext;
	if (GetInstigator())
	{
		CreationContext.AddTag(BotaniGameplayTags::Inventory::Item::Creation::Tag_ItemCreation_FromEntityDrop);
	}
	else
	{
		CreationContext.AddTag(BotaniGameplayTags::Inventory::Item::Creation::Tag_ItemCreation_FromWorld);
	}
	CreationContext.AddTag(BotaniGameplayTags::Inventory::Item::Creation::Tag_ItemCreation_FromPickup);
	return CreationContext;
}

void ABotaniPickupProxy::ResetPickupProxy()
{
	Super::ResetPickupProxy();

	// Reset the pickup glow color
	PickupLightComponent->SetLightColor(FColor::White);

	PickupDisplayMeshComponent->SetOverlayMaterial(nullptr);

	// Reset the pickup glow system color
	if (PickupGlowsSystem)
	{
		PickupGlowsSystem->SetVariableLinearColor("Color", FLinearColor::White);	
	}
}

void ABotaniPickupProxy::SetupPickupProxy(UCommonItemDefinition* NewItemDefinition)
{
	Super::SetupPickupProxy(NewItemDefinition);

	// Update the pickup glow color
	if (const UDataTable* ElementTable = CachedItemDefinition->PickupData.ElementDataTable)
	{
		for (const FName& RowName : ElementTable->GetRowNames())
		{
			const FBotaniElementData& ElementData = *ElementTable->FindRow<FBotaniElementData>(RowName, TEXT(""), true);
				
			if (ElementData.ElementTag.MatchesTagExact(CachedItemDefinition->PickupData.ElementTag))
			{
				ElementColor = ElementData.ElementColor;
				break;
			}
		}
	}

	PickupLightComponent->SetLightColor(ElementColor);

	if (UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(PickupGlowMaterial, PickupDisplayMeshComponent))
	{
		DynamicMaterial->SetVectorParameterValue("Color", ElementColor);
		PickupDisplayMeshComponent->SetOverlayMaterial(DynamicMaterial);
	}

	// Update the pickup glow system color
	PickupGlowsSystem->SetVariableLinearColor("Color", ElementColor);
}
