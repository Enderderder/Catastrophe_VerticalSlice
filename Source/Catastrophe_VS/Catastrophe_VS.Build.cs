// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Catastrophe_VS : ModuleRules
{
	public Catastrophe_VS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "UMG",
            "CableComponent", "ApexDestruction",
            "AIModule", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "CableComponent", "ApexDestruction" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
