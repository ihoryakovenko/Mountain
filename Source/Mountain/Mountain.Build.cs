// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Mountain : ModuleRules
{
	public Mountain(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
			"NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "DialogueManager", "UMG", });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
    }
}
