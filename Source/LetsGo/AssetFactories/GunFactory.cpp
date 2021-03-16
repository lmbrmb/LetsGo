#include "GunFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FString ASSET_PATH = "/Game/Assets/Blueprints/Weapons/";

GunFactory::GunFactory(const bool lazyInitialization)
{
	Paths.Add("SawedOffShotgun", AssetUtils::GenerateAssetPath(ASSET_PATH, "BP_SawedOffShotgun"));
	Paths.Add("Minigun", AssetUtils::GenerateAssetPath(ASSET_PATH, "BP_Minigun"));
	Paths.Add("Railgun", AssetUtils::GenerateAssetPath(ASSET_PATH, "BP_Railgun"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
