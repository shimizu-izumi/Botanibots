// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AbilitySystem/Abilities/Costs/BotaniAbilityCost_ItemTagStack.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/Abilities/BotaniEquipmentAbility.h"
#include "Inventory/Instance/CommonInventoryItemInstance.h"
#include "NativeGameplayTags.h"
#include "Botanibots/Botanibots.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BotaniAbilityCost_ItemTagStack)

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_FAIL_COST, "Ability.ActivateFail.Cost");


UBotaniAbilityCost_ItemTagStack::UBotaniAbilityCost_ItemTagStack()
{
	Quantity.SetValue(1.0f);
	FailureTag = TAG_ABILITY_FAIL_COST;
}

bool UBotaniAbilityCost_ItemTagStack::CheckCost(const UBotaniAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const UBotaniEquipmentAbility* EquipmentAbility = Cast<UBotaniEquipmentAbility>(Ability))
	{
		UCommonInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem();

		if (!ItemInstance)
		{
			ItemInstance = Cast<UCommonInventoryItemInstance>(Ability->GetSourceObject(Handle, ActorInfo));
			UE_LOG(LogBotaniAbilitySystem, Warning, TEXT("[%hs]: Failed to get AssociatedItem from EquipmentAbility. Falling back to SourceObject."), __FUNCTION__);
		}

		if (ItemInstance)
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);
			const float ScaledNumStacks = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(ScaledNumStacks);
			const bool bCanApplyCost = ItemInstance->GetStatTagStackCount(StackTag) >= NumStacks;

			if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
			{
				OptionalRelevantTags->AddTag(FailureTag);
			}

			return bCanApplyCost;
		}
	}

	return false;
}

void UBotaniAbilityCost_ItemTagStack::ApplyCost(const UBotaniAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// Only apply cost on server
	if (!ActorInfo->IsNetAuthority())
	{
		return;
	}

	if (const UBotaniEquipmentAbility* EquipmentAbility = Cast<UBotaniEquipmentAbility>(Ability))
	{
		UCommonInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem();

		if (!ItemInstance)
		{
			ItemInstance = Cast<UCommonInventoryItemInstance>(Ability->GetSourceObject(Handle, ActorInfo));
			UE_LOG(LogBotaniAbilitySystem, Warning, TEXT("[%hs]: Failed to get AssociatedItem from EquipmentAbility. Falling back to SourceObject."), __FUNCTION__);
		}

		if (ItemInstance)
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);
			const float ScaledNumStacks = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(ScaledNumStacks);

			ItemInstance->RemoveStatTagStack(StackTag, NumStacks);
		}
	}
}
