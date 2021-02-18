#pragma once

#include "InventoryItem.h"

///<summary>
/// Ammo item
///</summary>
class LETSGO_API AmmoItem final : public InventoryItem
{
public:
	explicit AmmoItem(FName id, bool isConsumable, int ammoAmount);

	int GetAmmoAmount() const;
	
private:
	int _ammoAmount;
};
