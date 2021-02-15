#include "WeaponInventoryItemFactory.h"

#include "WeaponInventoryItem.h"

WeaponInventoryItemFactory::WeaponInventoryItemFactory()
{
	_knownWeaponItems.Add("SawedOffShotgun");
	_knownWeaponItems.Add("Minigun");
}

WeaponInventoryItemFactory::~WeaponInventoryItemFactory()
{
	//Do nothing
}

InventoryItem* WeaponInventoryItemFactory::Create(FName itemId)
{
	if(_knownWeaponItems.Contains(itemId))
	{
		return new WeaponInventoryItem(itemId);
	}
	
	return nullptr;
}
