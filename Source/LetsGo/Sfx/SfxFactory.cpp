#include "SfxFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

FString SfxFactory::Q3AssetPath = "/Game/Assets/SoundEffects/Q3/";

FString SfxFactory::StepsAssetPath = Q3AssetPath + "Steps/";

FString SfxFactory::CharactersAssetPath = Q3AssetPath + "Characters/";

int SfxFactory::FirstStepIndex = 1;

int SfxFactory::StepsInGroup = 4;

SfxFactory::SfxFactory(
	IUObjectRegistry* uObjectRegistry,
	const bool lazyInitialization
) :	CachedAssetFactory<USoundBase>(uObjectRegistry)
{
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

	TArray<FString> characterIds;
	characterIds.Add("Uriel");
	characterIds.Add("Hunter");
	characterIds.Add("Minx");

	TArray<FString> characterSoundIds;
	characterSoundIds.Add("Jump1");
	characterSoundIds.Add("Death1");
	characterSoundIds.Add("Death2");
	characterSoundIds.Add("Death3");
	characterSoundIds.Add("Pain25");
	characterSoundIds.Add("Pain50");
	characterSoundIds.Add("Pain75");
	characterSoundIds.Add("Pain100");
	
	for (auto character : characterIds)
	{
		auto const characterAssetPath = CharactersAssetPath + character + "/";
		for(auto characterSoundId : characterSoundIds)
		{
			auto const assetPath = AssetUtils::GenerateAssetPath(characterAssetPath, "Sfx_q3_" + character + "_" + characterSoundId);
			Paths.Add(FName(character + characterSoundId), assetPath);
		}
	}
	
	if(!lazyInitialization)
	{
		LoadAllAssets();
	}
}
