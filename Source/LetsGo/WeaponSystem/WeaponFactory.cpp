#include "WeaponFactory.h"
#include "LetsGo/Utils/AssetUtils.h"

WeaponFactory::WeaponFactory()
{
	_weaponAssets.Add("SawedOffShotgun", "/Game/Assets/Blueprints/Weapons/BP_SawedOffShotgun");
	_weaponAssets.Add("Minigun", "/Game/Assets/Blueprints/Weapons/BP_Minigun");
	_weaponAssets.Add("Railgun", "/Game/Assets/Blueprints/Weapons/BP_Railgun");
}

WeaponFactory::~WeaponFactory()
{
	//Do nothing
}

UBlueprint* WeaponFactory::GetBlueprint(FName id)
{
	if(_weaponBlueprints.Contains(id))
	{
		return _weaponBlueprints[id];
	}

	auto const assetPath = _weaponAssets[id];
	auto const blueprint = AssetUtils::LoadBlueprint(assetPath);
	_weaponBlueprints.Add(id, blueprint);
	return blueprint;
}
