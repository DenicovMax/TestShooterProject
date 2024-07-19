// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SP : ModuleRules
{
	public SP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", 
			"CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", 
			"EnhancedInput", "UMG"});
	}
}
