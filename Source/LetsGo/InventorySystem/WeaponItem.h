#pragma once

#include "InventoryItem.h"

///<summary>
/// Weapon inventory item
///</summary>
class LETSGO_API WeaponItem final : public InventoryItem
{
public:
	explicit WeaponItem(
		const FName& id,
		const bool isConsumable,
		const bool isSingleInstance,
		const int quantity,
		const int maxQuantity
	);
};
