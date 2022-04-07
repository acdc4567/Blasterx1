// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Blaster : ModuleRules
{
	public Blaster(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
		
		PublicIncludePaths.AddRange(new string[]{
			"Blaster/Public/Character"
			,"Blaster/Public/GameMode"
			,"Blaster/Public/HUD"
			});
	
	}
}
