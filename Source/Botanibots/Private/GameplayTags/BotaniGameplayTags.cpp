// Copyright © 2024 Botanibots Team. All rights reserved.


#include "GameplayTags/BotaniGameplayTags.h"

/** Inventory Tags */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Inventory::Item::Creation::Tag_ItemCreation_FromPickup, "Inventory.Item.Creation.FromPickup", "Item creation tag for items created from pickups")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Inventory::Item::Creation::Tag_ItemCreation_FromBoss, "Inventory.Item.Creation.FromBoss", "Item creation tag for items dropped from bosses")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Inventory::Item::Creation::Tag_ItemCreation_FromEntityDrop, "Inventory.Item.Creation.FromEntityDrop", "Item creation tag for items dropped from entities")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Inventory::Item::Creation::Tag_ItemCreation_FromWorld, "Inventory.Item.Creation.FromWorld", "Item creation tag for items created in the world")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Inventory::Item::Type::Weapon::Tag_ItemType_BossWeapon, "Inventory.Item.Type.Weapon.BossWeapon", "Item type tag for boss weapons")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Inventory::Item::Type::Weapon::Tag_ItemType_Pistol, "Inventory.Item.Type.Weapon.Pistol", "Item type tag for pistols")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Inventory::Item::Type::Weapon::Tag_ItemType_Rifle, "Inventory.Item.Type.Weapon.Rifle", "Item type tag for rifles")

/** Ability Tags */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Ability::ActivateFail::Tag_ActivateFail_Cost, "Ability.ActivateFail.Cost", "Ability tag for failing to activate due to cost")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Ability::ActivateFail::Tag_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown", "Ability tag for failing to activate due to cooldown")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Ability::ActivateFail::Tag_ActivateFail_MagazineFull, "Ability.ActivateFail.MagazineFull", "Ability tag for failing to activate due to full magazine")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Ability::ActivateFail::Tag_ActivateFail_OutOfAmmo, "Ability.ActivateFail.OutOfAmmo", "Ability tag for failing to activate due to out of ammo")

/** UI Layer Tags */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::UI::Layer::Tag_Layer_Game, "UI.Layer.Game", "Game layer for UI elements")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::UI::Layer::Tag_Layer_Prompt, "UI.Layer.Prompt", "Prompt layer for UI elements")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::UI::Layer::Tag_Layer_Menu, "UI.Layer.Menu", "Menu layer for UI elements")

/** Input Tags */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTag_Jump, "Input.InputTag.Jump", "Jump input tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTag_Shoot, "Input.InputTag.Shoot", "Shoot input tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTag_Sprint, "Input.InputTag.Sprint", "Sprint input tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTag_Reload, "Input.InputTag.Reload", "Reload input tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTag_RMB, "Input.InputTag.RMB", "Right Mouse Button input tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTag_LMB, "Input.InputTag.LMB", "Left Mouse Button input tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_Move, "Input.InputTag.Native.Move", "Naitve Move input tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_Look, "Input.InputTag.Native.Look", "Native Look input tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_QuickAccess1, "Input.InputTag.Native.QuickAccess1", "Native Quick Access Slot 1 input tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_QuickAccess2, "Input.InputTag.Native.QuickAccess2", "Native Quick Access Slot 2 input tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::Input::InputTag::Tag_InputTagNative_QuickAccess3, "Input.InputTag.Native.QuickAccess3", "Native Quick Access Slot 3 input tag")

/** AI Behavior Tags */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::AI::Tag_AI_Behavior, "Botani.AI.Behavior", "AI Behavior tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::AI::Behavior::Tag_Behavior_AttackOnSight, "Botani.AI.Behavior.AttackOnSight", "AI Behavior tag for attacking on sight")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::AI::Behavior::Tag_Behavior_HoldPosition, "Botani.AI.Behavior.HoldPosition", "AI Behavior tag for holding position")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::AI::Behavior::Tag_Behavior_Aggressive, "Botani.AI.Behavior.Aggressive", "AI Behavior tag for being aggressive")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::AI::Behavior::Tag_Behavior_StayWithFriends, "Botani.AI.Behavior.StayWithFriends", "AI Behavior tag for staying with friends")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::AI::Behavior::Tag_Behavior_DefendFriends, "Botani.AI.Behavior.DefendFriends", "AI Behavior tag for defending friends")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BotaniGameplayTags::AI::Behavior::Tag_Behavior_RunAway, "Botani.AI.Behavior.RunAway", "AI Behavior tag for running away")
