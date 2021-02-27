#include "WeaponItem.h"

WeaponItem::WeaponItem(
	const FName& id,
	const bool isConsumable,
	const bool isSingleInstance,
	const int quantity,
	const int maxQuantity
)
	: InventoryItem(id, isConsumable, isSingleInstance, quantity, maxQuantity)
{
	//Do nothing
}