// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace BotaniGameplayTags
{
	namespace Inventory
	{
		namespace Item
		{
			namespace Creation
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ItemCreation_FromPickup)
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ItemCreation_FromBoss)
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ItemCreation_FromEntityDrop)
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ItemCreation_FromWorld)
			}

			namespace Type
			{
				namespace Weapon
				{
					UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ItemType_BossWeapon)
					UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ItemType_Pistol)
					UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ItemType_Rifle)
				}
			}
		}
	}
	namespace Ability
	{
		namespace ActivateFail
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ActivateFail_Cost)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ActivateFail_Cooldown)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ActivateFail_MagazineFull)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_ActivateFail_OutOfAmmo)
		}
	}
	
	namespace UI
	{
		namespace Layer
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Layer_Game)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Layer_Prompt)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Layer_Menu)
		}		
	}

	namespace Input
	{
		namespace InputTag
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTag_Jump);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTag_Shoot);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTag_Sprint);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTag_Reload);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTag_RMB);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTag_LMB);
			

			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTagNative_Move);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTagNative_Look);

			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTagNative_QuickAccess1);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTagNative_QuickAccess2);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InputTagNative_QuickAccess3);
		}
	}

	namespace AI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_AI_Behavior)
		namespace Behavior
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Behavior_AttackOnSight)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Behavior_HoldPosition)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Behavior_Aggressive)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Behavior_StayWithFriends)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Behavior_DefendFriends)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Behavior_RunAway)
		}
	}
}
