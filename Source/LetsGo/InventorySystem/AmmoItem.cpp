#include "AmmoItem.h"

AmmoItem::AmmoItem(FName id, bool isConsumable, int ammoAmount) :
	InventoryItem(id, isConsumable),
	_ammoAmount(ammoAmount)
{
	//Do nothing
}

int AmmoItem::GetAmmoAmount() const
{
	return _ammoAmount;
}
