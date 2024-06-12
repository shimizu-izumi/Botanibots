// Some copyright should be here...

using UnrealBuildTool;

public class CommonEquipment : ModuleRules
{
	public CommonEquipment(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"NetCore",
				"GameplayTags",
				"Niagara",
				"UMG",
			});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"GameplayMessageRuntime",
				"GameplayAbilities",
			});
	}
}
