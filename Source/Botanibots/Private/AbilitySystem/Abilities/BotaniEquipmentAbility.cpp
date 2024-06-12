// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AbilitySystem/Abilities/BotaniEquipmentAbility.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UBotaniEquipmentAbility::UBotaniEquipmentAbility(const FObjectInitializer& ObjectInitializer)
	: UBotaniAbility(ObjectInitializer)
{
}

UCommonInventoryItemInstance* UBotaniEquipmentAbility::GetAssociatedItem() const
{
	if (const FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UCommonInventoryItemInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

UCommonInventoryItemInstance* UBotaniEquipmentAbility::GetAssociatedItemOfClass(TSubclassOf<UCommonInventoryItemInstance> ItemClass) const
{
	if (const FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		if (ItemClass->IsChildOf(Spec->SourceObject.Get()->GetClass()))
		{
			return Cast<UCommonInventoryItemInstance>(Spec->SourceObject.Get());
		}
	}
	return nullptr;
}

#if WITH_EDITOR
EDataValidationResult UBotaniEquipmentAbility::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		Context.AddError(NSLOCTEXT("Botani", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}
	
	return Result;
}
#endif