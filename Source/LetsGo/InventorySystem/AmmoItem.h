#pragma once

#include "InventoryItem.h"

///<summary>
/// Ammo item
///</summary>
class LETSGO_API AmmoItem final : public InventoryItem
{
public:
	explicit AmmoItem(
		const FName& id,
		const bool isConsumable,
		const bool isSingleInstance,
		const int quantity,
		const int maxQuantity
	);
};
