#include "GunItemFactory.h"

GunItemFactory::GunItemFactory()
{
	_gunData.Add("SawedOffShotgun", GunData(WeaponType("Shotgun"), "ShotgunShells", 10));
	_gunData.Add("Minigun", GunData(WeaponType("Machinegun"), "MachinegunBullets", 75));
	const WeaponType railgunWeaponType("Railgun");
	_gunData.Add("Railgun", GunData(railgunWeaponType, "RailgunBullets", 10));
	_gunData.Add("RailgunQ3", GunData(railgunWeaponType, "RailgunBullets", 10));
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
