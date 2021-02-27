#include "AmmoItemFactory.h"
#include "AmmoItem.h"

AmmoItemFactory::AmmoItemFactory()
{
	_ammoData.Add("ShotgunShells", AmmoData(10, 30));
	_ammoData.Add("RailgunBullets", AmmoData(10, 30));
	_ammoData.Add("MachinegunBullets", AmmoData(50, 150));
}

AmmoItemFactory::~AmmoItemFactory()
{
	//Do nothing
}

InventoryItem* AmmoItemFactory::Create(const FName itemId)
{
	if(_ammoData.Contains(itemId))
	{
		auto const ammoData = _ammoData[itemId];
		return new AmmoItem(itemId, false, true, ammoData.Quantity, ammoData.MaxQuantity);
	}
	
	return nullptr;
}
