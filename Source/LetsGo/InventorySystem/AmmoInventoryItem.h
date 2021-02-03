#pragma once

#include "InventoryItem.h"

///<summary>
/// Ammo inventory item
///</summary>
class LETSGO_API AmmoInventoryItem final : public InventoryItem
{
public:
	explicit AmmoInventoryItem(FName id);
	
	~AmmoInventoryItem();
};
