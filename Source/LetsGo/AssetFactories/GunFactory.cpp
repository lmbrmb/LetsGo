#include "GunFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FString GunFactory::AssetPath = "/Game/Assets/Blueprints/Guns/";

GunFactory::GunFactory(
	IUObjectRegistry* uObjectRegistry,
	const bool lazyInitialization
	
) : CachedAssetFactory<UBlueprintGeneratedClass>(uObjectRegistry)
{
	Paths.Add("ShotgunQ3", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_ShotgunQ3"));
	Paths.Add("Minigun", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_Minigun"));
	Paths.Add("MachinegunQ3", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_MachinegunQ3"));
	Paths.Add("RailgunQ3", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_RailgunQ3"));
	Paths.Add("RocketLauncherQ3", AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_RocketLauncherQ3"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
