// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Inventory/Instance/CommonInventoryItemInstance.h"

#include "GameplayTagContainer.h"
#include "GameplayTagStacks/GameplayTagStackContainer.h"
#include "Inventory/Component/CommonInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonInventoryItemInstance)

UCommonInventoryItemInstance::UCommonInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UCommonItemDefinition* UCommonInventoryItemInstance::GetItemDef() const
{
	return ItemDefinition;
}

void UCommonInventoryItemInstance::SetItemDef(UCommonItemDefinition* ItemDef)
{
	ItemDefinition = ItemDef;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, ItemDefinition, this);
}

void UCommonInventoryItemInstance::SetOwnerComponent(UActorComponent* Owner)
{
	OwnerComponent = Owner;
}

int32 UCommonInventoryItemInstance::GetItemStackCount()
{
	if (UCommonInventoryComponent* InventoryComponent = Cast<UCommonInventoryComponent>(OwnerComponent))
	{
		return InventoryComponent->GetItemStackCount(this);
	}
	return INDEX_NONE;
}

void UCommonInventoryItemInstance::OnInstanceCreated(UCommonItemDefinition* ItemDef, UActorComponent* OwningComponent, FGameplayTagContainer CreationContext)
{
	TagStacks.OnStackValueChangedDelegate.AddLambda(
		[this] (const FGameplayTagStack& Stack)
		{
			OnStackValueChangedDelegate.Broadcast(Stack);
		});
}

void UCommonInventoryItemInstance::OnEquipped()
{
}

void UCommonInventoryItemInstance::OnUnequipped()
{
}

void UCommonInventoryItemInstance::AddStatTagStack(FGameplayTag StatTag, int32 StackCount, bool bPersistentStat)
{
	TagStacks.AddStack(StatTag, StackCount, bPersistentStat);
}

void UCommonInventoryItemInstance::RemoveStatTagStack(FGameplayTag StatTag, int32 StackCount)
{
	TagStacks.RemoveStack(StatTag, StackCount);
}

int32 UCommonInventoryItemInstance::GetStatTagStackCount(FGameplayTag StatTag) const
{
	return TagStacks.GetStackCount(StatTag);
}

bool UCommonInventoryItemInstance::HasTagStack(FGameplayTag StatTag) const
{
	return TagStacks.ContainsStack(StatTag);
}

void UCommonInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedLifetimeParams;
	SharedLifetimeParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ItemDefinition, SharedLifetimeParams);
	DOREPLIFETIME(ThisClass, TagStacks);
}
