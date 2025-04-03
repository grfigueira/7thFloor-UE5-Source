// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PristsWithGuns : ModuleRules
{
	public PristsWithGuns(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "EngineCameras", "ActorSequence" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"FMODStudio",
			"FMODStudio",
			"PhysicsCore",
			"UMG",
			"FCTween", // TODO I'm not currently using this
			"Json",
			"Slate",
			"SlateCore" 
		});
	}
}