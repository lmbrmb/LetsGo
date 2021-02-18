#include "HealthItem.h"

HealthItem::HealthItem(FName id, bool isConsumable, float healthAmount) :
	InventoryItem(id, isConsumable),
	_healAmount(healthAmount)
{
	//Do nothing
}

int HealthItem::GetHealAmount() const
{
	return _healAmount;
}
