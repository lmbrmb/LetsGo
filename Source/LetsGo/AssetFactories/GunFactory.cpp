#include "GunFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FString GunFactory::_assetPath = "/Game/Assets/Blueprints/Guns/";

GunFactory::GunFactory(const bool lazyInitialization)
{
	Paths.Add("SawedOffShotgun", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_SawedOffShotgun"));
	Paths.Add("ShotgunQ3", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_ShotgunQ3"));
	Paths.Add("Minigun", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_Minigun"));
	Paths.Add("MachinegunQ3", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_MachinegunQ3"));
	Paths.Add("Railgun", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_Railgun"));
	Paths.Add("RailgunQ3", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_RailgunQ3"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
