#include "PickupItemFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FString PickupItemFactory::_assetPath = "/Game/Assets/Blueprints/PickupItems/";

PickupItemFactory::PickupItemFactory(const bool lazyInitialization)
{
	Paths.Add("SawedOffShotgun", AssetUtils::GenerateAssetPath(_assetPath, "BP_SawedOffShotgunPickupItem"));
	Paths.Add("Minigun", AssetUtils::GenerateAssetPath(_assetPath, "BP_MinigunPickupItem"));
	Paths.Add("Railgun", AssetUtils::GenerateAssetPath(_assetPath, "BP_RailgunPickupItem"));
	Paths.Add("MajorHealth", AssetUtils::GenerateAssetPath(_assetPath, "BP_MajorHealthPickupItem"));
	Paths.Add("MinorHealth", AssetUtils::GenerateAssetPath(_assetPath, "BP_MinorHealthPickupItem"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
