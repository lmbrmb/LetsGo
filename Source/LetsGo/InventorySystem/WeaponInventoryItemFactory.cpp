#include "WeaponInventoryItemFactory.h"

#include "WeaponInventoryItem.h"

WeaponInventoryItemFactory::WeaponInventoryItemFactory()
{
}

WeaponInventoryItemFactory::~WeaponInventoryItemFactory()
{
}

InventoryItem* WeaponInventoryItemFactory::Create(FName itemId)
{
	if(itemId == "SawedOffShotgun")
	{
		return new WeaponInventoryItem(itemId);
	}
	
	return nullptr;
}
