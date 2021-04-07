#include "PickupItemFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FString PickupItemFactory::_assetPath = "/Game/Assets/Blueprints/PickupItems/";

PickupItemFactory::PickupItemFactory(const bool lazyInitialization)
{
	//Weapons
	Paths.Add("SawedOffShotgun", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_SawedOffShotgunPickupItem"));
	Paths.Add("ShotgunQ3", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_ShotgunQ3PickupItem"));
	Paths.Add("Minigun", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_MinigunPickupItem"));
	Paths.Add("MachinegunQ3", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_MachinegunQ3PickupItem"));
	Paths.Add("Railgun", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_RailgunPickupItem"));
	Paths.Add("RailgunQ3", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_RailgunQ3PickupItem"));

	//Health
	Paths.Add("MajorHealth", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_MajorHealthPickupItem"));
	Paths.Add("MinorHealth", AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_MinorHealthPickupItem"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
