// Copyright © 2024 Botanibots Team. All rights reserved.


#include "EquipmentSystem/Inventory/Fragments/Weapon/WeaponFragment_RangedWeapon.h"

UWeaponFragment_RangedWeapon::UWeaponFragment_RangedWeapon()
{
	WeaponType = ECommonItemType::WeaponRanged;
}

void UWeaponFragment_RangedWeapon::OnInstanceCreated(UCommonInventoryItemInstance* ItemInstance, const FGameplayTagContainer& CreationContext)
{
	Super::OnInstanceCreated(ItemInstance, CreationContext);

	if (AmmoData)
	{
	}
}
