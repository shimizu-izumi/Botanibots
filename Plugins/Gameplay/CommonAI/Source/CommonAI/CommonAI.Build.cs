// Some copyright should be here...

using UnrealBuildTool;

public class CommonAI : ModuleRules
{
	public CommonAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"AIModule",
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks"
			});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"AIModule"
			});
	}
}
