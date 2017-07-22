// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSProtoSandboxEditor : ModuleRules
{

	public FPSProtoSandboxEditor(TargetInfo Target)
	{
        PrivateIncludePaths.AddRange(new string[]{"FPSProtoSandboxEditor/Private"});
        PublicIncludePaths.AddRange(new string[]{"FPSProtoSandboxEditor/Public"});
        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("UnrealEd");
        PublicDependencyModuleNames.Add("FPSProtoSandbox");
	}
}
