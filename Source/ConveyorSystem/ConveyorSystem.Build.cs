// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ConveyorSystem : ModuleRules
{
	public ConveyorSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"JCore" });
	}
}
