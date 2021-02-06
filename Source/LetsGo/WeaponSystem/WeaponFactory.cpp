#include "WeaponFactory.h"
#include "LetsGo/Utils/AssetUtils.h"

WeaponFactory::WeaponFactory()
{
	_weaponAssetDictionary.Add("SawedOffShotgun", "/Game/Assets/Blueprints/Weapons/SawedOffShotgun");
}

WeaponFactory::~WeaponFactory()
{
}

AWeaponBase* WeaponFactory::Create(AActor* owner, FName id)
{
	auto const assetPath = _weaponAssetDictionary[id];
	auto const blueprint = AssetUtils::LoadBlueprint(assetPath);
	// TODO: blueprint is loaded but not unloaded. Potential memory leak?
	auto weapon = AssetUtils::Spawn<AWeaponBase>(owner, blueprint, owner->GetActorLocation(), owner->GetActorRotation());
	return weapon;
}
