#pragma once

#include "InventoryItem.h"

///<summary>
/// Weapon inventory item
///</summary>
class LETSGO_API WeaponItem final : public InventoryItem
{
public:
	explicit WeaponItem(FName id, bool isConsumable);
};
