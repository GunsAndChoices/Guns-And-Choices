using UnrealBuildTool;

public class Guns_And_ChoicesTarget : TargetRules
{
	public Guns_And_ChoicesTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Guns_And_Choices");
	}
}
