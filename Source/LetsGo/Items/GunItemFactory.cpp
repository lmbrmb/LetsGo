#include "GunItemFactory.h"

GunItemFactory::GunItemFactory()
{
	_gunData.Add("SawedOffShotgun", GunData("ShotgunShells", 10));
	_gunData.Add("Minigun", GunData("MachinegunBullets", 100));
	_gunData.Add("Railgun", GunData("RailgunBullets", 10));
}

GunItem* GunItemFactory::CreateItem(const FName itemId)
{
	if (!_gunData.Contains(itemId))
	{
		return nullptr;
	}
	
	auto const gunData = _gunData[itemId];
	auto const gunItem = new GunItem(itemId, gunData.AmmoType, gunData.InitialAmmoCount);;
	return gunItem;
}
