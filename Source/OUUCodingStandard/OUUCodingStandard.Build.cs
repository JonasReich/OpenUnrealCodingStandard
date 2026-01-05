// Copyright (c) 2025 Jonas Reich

using UnrealBuildTool;

public class OUUCodingStandard : ModuleRules
{
	public OUUCodingStandard(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		IWYUSupport = IWYUSupport.Full;

#if UE_5_4_OR_LATER
		bWarningsAsErrors = true;
#endif

		// [build.cs.dep] Prefer private over public dependenices.
		// This gives a better overview of what dependencies need to be included for dependents / how clean your modules public API is.
		// It should also improve compile/include time, but that's probably negligible.
		// Usually you can stick to the following pattern:
		// - Is this module included in the header? -> use a public dependency
		// - Is this module included only in source files (implementation detail) -> use a private dependency
		// Keep in mind: Dependency modules are implicitly transferred to dependents for the COMPILER, but not for the LINKER.
		// -> In all cases, dependents of this module will need to re-declare a linker dependency explicitly.
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);

		PrivateDependencyModuleNames.Add("GameplayTags");
	}
}