using UnrealBuildTool;

public class whysoserious : ModuleRules
{
	public whysoserious(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"UMG"
			}
		);

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
