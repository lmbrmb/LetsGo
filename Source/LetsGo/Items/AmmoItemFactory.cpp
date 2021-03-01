#include "AmmoItemFactory.h"

AmmoItemFactory::AmmoItemFactory()
{
	_ammoData.Add("ShotgunShells", AmmoData(10, 30));
	_ammoData.Add("RailgunBullets", AmmoData(10, 30));
	_ammoData.Add("MachinegunBullets", AmmoData(50, 150));
}

AmmoItem* AmmoItemFactory::CreateItem(const FName itemId)
{
	if (!_ammoData.Contains(itemId))
	{
		return nullptr;
	}
	
	auto const ammoData = _ammoData[itemId];
	auto const ammoItem = new AmmoItem(itemId, ammoData.Quantity, ammoData.MaxQuantity);
	return ammoItem;
}
