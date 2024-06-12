using UnrealBuildTool;

public class CommonEquipmentEditor : ModuleRules
{
    public CommonEquipmentEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "AssetTools",
                "AssetDefinition",
                "SlateCore",
                "Slate",
                "EditorStyle",
                "UnrealEd", 
                "PluginUtils",
                "ContentBrowser",
                "Projects",
                "UMG",
                "DeveloperSettings",
                "CommonEquipment"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "CommonEquipment",
            }
        );
    }
}