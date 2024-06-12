using UnrealBuildTool;

public class CommonAIEditor : ModuleRules
{
    public CommonAIEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "AdvancedPreviewScene",
                "Core",
                "CoreUObject",
                "ComponentVisualizers",
                "EditorInteractiveToolsFramework",
                "Engine",
                "InputCore",
                "InteractiveToolsFramework",
                "PropertyEditor",
                "SlateCore",
                "Slate",
                "CommonAI",
                "SourceControl",
                "UnrealEd",
                "StructUtils",
                "AIModule"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "RenderCore",
                "ApplicationCore",
                "AssetDefinition",
                "StructUtilsEditor",
            }
        );
    }
}