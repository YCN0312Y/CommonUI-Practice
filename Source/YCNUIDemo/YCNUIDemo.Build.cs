// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class YCNUIDemo : ModuleRules
{
	public YCNUIDemo(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "UMG",
            "Slate",
            "SlateCore",
            "CommonUI",
            "CommonInput",
            "GameplayTags",
        });
    }
}
