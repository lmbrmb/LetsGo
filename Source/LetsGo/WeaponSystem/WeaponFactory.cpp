#include "WeaponFactory.h"
#include "LetsGo/Utils/AssetUtils.h"

WeaponFactory::WeaponFactory()
{
	_weaponAssetDictionary.Add("SawedOffShotgun", "/Game/Assets/Blueprints/Weapons/SawedOffShotgun");
	_weaponAssetDictionary.Add("Minigun", "/Game/Assets/Blueprints/Weapons/Minigun");
}

WeaponFactory::~WeaponFactory()
{
}

UBlueprint* WeaponFactory::GetBlueprint(FName id)
{
	if(_weaponBlueprintsDictionary.Contains(id))
	{
		return _weaponBlueprintsDictionary[id];
	}

	auto const assetPath = _weaponAssetDictionary[id];
	auto const blueprint = AssetUtils::LoadBlueprint(assetPath);
	_weaponBlueprintsDictionary.Add(id, blueprint);
	return blueprint;
}
