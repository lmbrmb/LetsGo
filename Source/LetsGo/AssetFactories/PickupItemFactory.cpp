#include "PickupItemFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FString PickupItemFactory::AssetPath = "/Game/Assets/Blueprints/PickupItems/";

PickupItemFactory::PickupItemFactory(
	IUObjectRegistry* uObjectRegistry,
	const bool lazyInitialization
) : CachedAssetFactory<UBlueprintGeneratedClass>(uObjectRegistry)
{
	//Weapons
	Paths.Add("ShotgunQ3", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_ShotgunQ3PickupItem"));
	Paths.Add("Minigun", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_MinigunPickupItem"));
	Paths.Add("MachinegunQ3", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_MachinegunQ3PickupItem"));
	Paths.Add("RailgunQ3", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_RailgunQ3PickupItem"));
	Paths.Add("RocketLauncherQ3", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_RocketLauncherQ3PickupItem"));

	//Health
	Paths.Add("MajorHealth", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_MajorHealthPickupItem"));
	Paths.Add("MinorHealth", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_MinorHealthPickupItem"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
