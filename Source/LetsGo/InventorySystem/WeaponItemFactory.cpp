#include "WeaponItemFactory.h"
#include "WeaponItem.h"

WeaponItemFactory::WeaponItemFactory()
{
	_knownWeaponItems.Add("SawedOffShotgun");
	_knownWeaponItems.Add("Minigun");
	_knownWeaponItems.Add("Railgun");
}

WeaponItemFactory::~WeaponItemFactory()
{
	//Do nothing
}

InventoryItem* WeaponItemFactory::Create(FName itemId)
{
	if(_knownWeaponItems.Contains(itemId))
	{
		return new WeaponItem(itemId, false);
	}
	
	return nullptr;
}
