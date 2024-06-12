// Copyright © 2024 Botanibots Team. All rights reserved.


#include "EquipmentSystem/Inventory/Instance/BotaniInventoryWeaponInstance.h"

#include "EquipmentSystem/Inventory/Definition/BotaniWeaponDefinition.h"
#include "EquipmentSystem/Inventory/Fragments/BotaniWeaponFragment.h"

void UBotaniInventoryWeaponInstance::OnInstanceCreated(UCommonItemDefinition* ItemDef, UActorComponent* OwningComponent, FGameplayTagContainer CreationContext)
{
	Super::OnInstanceCreated(ItemDef, OwningComponent, CreationContext);

	for (const auto& Weapon : Cast<UBotaniWeaponDefinition>(ItemDef)->WeaponTypes)
	{
		if (Weapon)
		{
			Weapon->OnInstanceCreated(this, CreationContext);
		}
	}
}

UBotaniWeaponFragment* UBotaniInventoryWeaponInstance::GetWeaponDefinitionByClass(TSubclassOf<UBotaniWeaponFragment> WeaponClass) const
{
	if (!WeaponClass)
	{
		WeaponClass = UBotaniWeaponFragment::StaticClass();
	}

	for (UBotaniWeaponFragment* Weapon : Cast<UBotaniWeaponDefinition>(GetItemDef())->WeaponTypes)
	{
		if (Weapon->IsA(WeaponClass))
		{
			return Weapon;
		}
	}

	return nullptr;
}
