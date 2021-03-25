#include "GunFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FString GunFactory::_assetPath = "/Game/Assets/Blueprints/Guns/";

GunFactory::GunFactory(const bool lazyInitialization)
{
	Paths.Add("SawedOffShotgun", AssetUtils::GenerateAssetPath(_assetPath, "BP_SawedOffShotgun"));
	Paths.Add("Minigun", AssetUtils::GenerateAssetPath(_assetPath, "BP_Minigun"));
	Paths.Add("Railgun", AssetUtils::GenerateAssetPath(_assetPath, "BP_Railgun"));
	Paths.Add("RailgunQ3", AssetUtils::GenerateAssetPath(_assetPath, "BP_RailgunQ3"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
