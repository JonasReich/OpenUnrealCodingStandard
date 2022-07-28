// Copyright (c) 2022 Jonas Reich
// [build.cs.copyright] Every Build.cs file must start with the copy right notice above

using UnrealBuildTool;

public class OUUCodingStandard : ModuleRules
{
	public OUUCodingStandard(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// [build.cs.dep] Prefer declaring dependencies as private if possible.
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);
	}
}