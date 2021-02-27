#include "HealthItem.h"

HealthItem::HealthItem(
	const FName& id,
	const bool isConsumable,
	const bool isSingleInstance,
	const int quantity,
	const int maxQuantity,
	const float healAmount
)
	: InventoryItem(id, isConsumable, isSingleInstance, quantity, maxQuantity),
	_healAmount(healAmount)
{
	//Do nothing
}

int HealthItem::GetHealAmount() const
{
	return _healAmount;
}
