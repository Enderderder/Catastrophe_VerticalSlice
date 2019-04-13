// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Catastrophe_VSEditorTarget : TargetRules
{
	public Catastrophe_VSEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Catastrophe_VS" } );
	}
}
