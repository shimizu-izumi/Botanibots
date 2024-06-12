// Copyright © 2024 MajorT. All rights reserved.


#include "Inventory/Data/InventoryConfig.h"

#include "Inventory/Component/CommonEquipmentComponent.h"
#include "Inventory/Component/CommonInventoryComponent.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

void UInventoryConfigFragment::PostInventoryInitialize(UCommonInventoryComponent* InventoryComponent, AActor* InOwnerActor, AActor* InAvatarActor)
{
	TSubclassOf<UCommonEquipmentBaseComponent> AdditionalComponentClass = GetAdditionalComponentClass();
	if (AdditionalComponentClass != nullptr)
	{
		UActorComponent* NewComponent = InOwnerActor->AddComponentByClass(AdditionalComponentClass, false, FTransform::Identity, false);
		UCommonEquipmentBaseComponent* EquipmentComponent = Cast<UCommonEquipmentBaseComponent>(NewComponent);
		InventoryComponent->AddChildComponent(EquipmentComponent);
	}
}

#if WITH_EDITOR
EDataValidationResult UInventoryConfig::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	for (const FCommonInitialItemConfig& InitialItemConfig : InitialItems)
	{
		if (InitialItemConfig.Item == nullptr)
		{
			Context.AddError(NSLOCTEXT("CommonEquipment", "InitialItemConfigItemNull", "Initial item config item is null"));
			Result = EDataValidationResult::Invalid;
			break;
		}

		if (InitialItemConfig.bEquipItem)
		{
			bool bHasEquipmentComponent = false;
			for (const auto& Config : InventoryConfigurations)
			{
				if (Config->GetAdditionalComponentClass() == nullptr)
				{
					Context.AddError(NSLOCTEXT("CommonEquipment", "InitialItemConfigEquipNoEquipmentComponent", "Initial item config is set to equip item but no equipment component is available"));
					Result = EDataValidationResult::Invalid;
					break;
				}

				if (Config->GetAdditionalComponentClass() == UCommonEquipmentComponent::StaticClass())
				{
					bHasEquipmentComponent = true;
				}
			}

			if (bHasEquipmentComponent == false)
			{
				Context.AddError(NSLOCTEXT("CommonEquipment", "InitialItemConfigEquipNoEquipmentComponent", "Initial item config is set to equip item but no equipment component is available"));
				Result = EDataValidationResult::Invalid;
				break;
			}
		}
	}
	
	return Result;
}
#endif
