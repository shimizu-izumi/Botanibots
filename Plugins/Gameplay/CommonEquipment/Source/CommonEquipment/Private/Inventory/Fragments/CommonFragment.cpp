// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Inventory/Fragments/CommonFragment.h"

UCommonFragment::UCommonFragment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCommonFragment::OnInstanceCreated(UCommonInventoryItemInstance* ItemInstance, const FGameplayTagContainer& CreationContext)
{
	K2_OnInstanceCreated(ItemInstance, CreationContext);
}

void UCommonFragment::OnInstanceDestroyed(UCommonInventoryItemInstance* ItemInstance)
{
	K2_OnInstanceDestroyed(ItemInstance);
}

bool UCommonFragment::IsFragmentValidForInstance(UCommonInventoryItemInstance* ItemInstance) const
{
	return K2_IsFragmentValidForInstance(ItemInstance);
}

bool UCommonFragment::IsFragmentValidForContext(const FGameplayTagContainer& CreationContext) const
{
	return K2_IsFragmentValidForContext(CreationContext);
}

bool UCommonFragment::OnlyForInitialInstance() const
{
	return K2_OnlyForInitialInstance();
}

bool UCommonFragment::K2_OnlyForInitialInstance_Implementation() const
{
	return false;
}

bool UCommonFragment::K2_IsFragmentValidForContext_Implementation(const FGameplayTagContainer& CreationContext) const
{
	return true;
}

bool UCommonFragment::K2_IsFragmentValidForInstance_Implementation(UCommonInventoryItemInstance* ItemInstance) const
{
	return true;
}

void UCommonFragment::K2_OnInstanceDestroyed_Implementation(UCommonInventoryItemInstance* ItemInstance)
{
}

void UCommonFragment::K2_OnInstanceCreated_Implementation(UCommonInventoryItemInstance* ItemInstance, const FGameplayTagContainer& CreationContext)
{
}
