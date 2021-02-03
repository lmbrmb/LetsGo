#pragma once

#include "InventoryItem.h"

///<summary>
/// Weapon inventory item
///</summary>
class LETSGO_API WeaponInventoryItem final : public InventoryItem
{
public:
	explicit WeaponInventoryItem(FName id);
	
	~WeaponInventoryItem();
};
