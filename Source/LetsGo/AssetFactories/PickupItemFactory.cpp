#include "PickupItemFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FString ASSET_PATH = "/Game/Assets/Blueprints/PickupItems/";

PickupItemFactory::PickupItemFactory(const bool lazyInitialization)
{
	Paths.Add("SawedOffShotgun", AssetUtils::GenerateAssetPath(ASSET_PATH, "BP_SawedOffShotgunPickupItem"));
	Paths.Add("Minigun", AssetUtils::GenerateAssetPath(ASSET_PATH, "BP_MinigunPickupItem"));
	Paths.Add("Railgun", AssetUtils::GenerateAssetPath(ASSET_PATH, "BP_RailgunPickupItem"));
	Paths.Add("MajorHealth", AssetUtils::GenerateAssetPath(ASSET_PATH, "BP_MajorHealthPickupItem"));
	Paths.Add("MinorHealth", AssetUtils::GenerateAssetPath(ASSET_PATH, "BP_MinorHealthPickupItem"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
