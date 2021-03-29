#include "PickupItemFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FString PickupItemFactory::_assetPath = "/Game/Assets/Blueprints/PickupItems/";

PickupItemFactory::PickupItemFactory(const bool lazyInitialization)
{
	//Weapons
	Paths.Add("SawedOffShotgun", AssetUtils::GenerateAssetPath(_assetPath, "BP_SawedOffShotgunPickupItem"));
	Paths.Add("ShotgunQ3", AssetUtils::GenerateAssetPath(_assetPath, "BP_ShotgunQ3PickupItem"));
	Paths.Add("Minigun", AssetUtils::GenerateAssetPath(_assetPath, "BP_MinigunPickupItem"));
	Paths.Add("MachinegunQ3", AssetUtils::GenerateAssetPath(_assetPath, "BP_MachinegunQ3PickupItem"));
	Paths.Add("Railgun", AssetUtils::GenerateAssetPath(_assetPath, "BP_RailgunPickupItem"));
	Paths.Add("RailgunQ3", AssetUtils::GenerateAssetPath(_assetPath, "BP_RailgunQ3PickupItem"));

	//Health
	Paths.Add("MajorHealth", AssetUtils::GenerateAssetPath(_assetPath, "BP_MajorHealthPickupItem"));
	Paths.Add("MinorHealth", AssetUtils::GenerateAssetPath(_assetPath, "BP_MinorHealthPickupItem"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
