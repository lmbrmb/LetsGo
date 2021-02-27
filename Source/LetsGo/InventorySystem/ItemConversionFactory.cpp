#include "ItemConversionFactory.h"

ItemConversionFactory::ItemConversionFactory()
{
	_itemToItem.Add("SawedOffShotgun", "ShotgunShells");
	_itemToItem.Add("Minigun", "MachinegunBullets");
	_itemToItem.Add("Railgun", "RailgunBullets");
}

FName ItemConversionFactory::GetConvertedItemId(const FName itemId)
{
	if(_itemToItem.Contains(itemId))
	{
		return _itemToItem[itemId];
	}
	return FName();
}
