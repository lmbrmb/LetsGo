#include "GunItemFactory.h"

GunItemFactory::GunItemFactory()
{
	const WeaponType shotgunWeaponType("Shotgun");
	_gunData.Add("SawedOffShotgun", GunData(shotgunWeaponType, "ShotgunShells", 10));
	_gunData.Add("ShotgunQ3", GunData(shotgunWeaponType, "ShotgunShells", 10));

	const WeaponType machinegunWeaponType("Machinegun");
	_gunData.Add("Minigun", GunData(machinegunWeaponType, "MinigunBullets", 75));
	_gunData.Add("MachinegunQ3", GunData(machinegunWeaponType, "MachinegunBullets", 999));
	
	const WeaponType railgunWeaponType("Railgun");
	_gunData.Add("Railgun", GunData(railgunWeaponType, "RailgunBullets", 10));
	_gunData.Add("RailgunQ3", GunData(railgunWeaponType, "RailgunBullets", 10));

	const WeaponType rocketLauncherWeaponType("RocketLauncher");
	_gunData.Add("RocketLauncherQ3", GunData(rocketLauncherWeaponType, "Rockets", 10));
}

GunItem* GunItemFactory::CreateItem(const FName itemId)
{
	if (!_gunData.Contains(itemId))
	{
		return nullptr;
	}

	auto const gunData = _gunData[itemId];
	auto const gunItem = new GunItem(itemId, gunData.WeaponType, gunData.AmmoId, gunData.InitialAmmoCount);;
	return gunItem;
}
