#include "SfxFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

FString SfxFactory::StepsAssetPath = "/Game/Assets/SoundEffects/Q3/Steps/";

FString SfxFactory::JumpsAssetPath = "/Game/Assets/SoundEffects/Q3/Jumps/";

int SfxFactory::FirstStepIndex = 1;

int SfxFactory::StepsInGroup = 4;

SfxFactory::SfxFactory()
{
	// Jump
	Paths.Add("Jump1", AssetUtils::GenerateAssetPath(JumpsAssetPath, "Sfx_q3_jump1"));
	Paths.Add("Jump2", AssetUtils::GenerateAssetPath(JumpsAssetPath, "Sfx_q3_jump1"));
	Paths.Add("Jump3", AssetUtils::GenerateAssetPath(JumpsAssetPath, "Sfx_q3_jump1"));

	// Land
	Paths.Add("Land", AssetUtils::GenerateAssetPath(StepsAssetPath, "Sfx_q3_land1"));

	// Steps
	
	// Example:
	// Key="StepBoot1", Value="Sfx_q3_boot1"
	// Key="StepMech3", Value="Sfx_q3_mech3"

	TMap<FString, FString> stepSoundDict;
	stepSoundDict.Add("StepNormal", "Sfx_q3_step");
	stepSoundDict.Add("StepBoot", "Sfx_q3_boot");
	stepSoundDict.Add("StepClank", "Sfx_q3_clank");
	stepSoundDict.Add("StepFlesh", "Sfx_q3_flesh");
	stepSoundDict.Add("StepMech", "Sfx_q3_mech");
	stepSoundDict.Add("StepSplash", "Sfx_q3_splash");
	
	for (auto i = FirstStepIndex; i <= StepsInGroup; i++)
	{
		for (auto stepSoundRecord : stepSoundDict)
		{
			auto const baseKey = stepSoundRecord.Key;
			auto const baseAssetName = stepSoundRecord.Value;

			auto const soundIndexStr = UKismetStringLibrary::Conv_IntToString(i);
			auto const key = FName(baseKey + soundIndexStr);
			auto const assetName = baseAssetName + soundIndexStr;
			auto const value = AssetUtils::GenerateAssetPath(StepsAssetPath, assetName);
			Paths.Add(key, value);
		}
	}

	LoadAllAssets();
}
