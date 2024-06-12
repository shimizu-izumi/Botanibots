// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Botanibots : ModuleRules
{
	public Botanibots(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreOnline",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"CommonUI",
			"CommonEquipment",
			"Niagara",
			"NetCore",
			"PhysicsCore",
			"Networking",
			"DataRegistry",
			"ReplicationGraph",
			"ModularGameplayActors"
		});

		// Gameplay Ability System
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
		});
		
		// AI
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"AIModule",
			"CommonAI",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Niagara",
			"NetCore",
			"Networking",
			"ModularGameplay",
			"NetCore",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"OnlineSubsystemEOS"
		});
		
		PrivateDefinitions.Add("P2PMODE=1");
	}
}
