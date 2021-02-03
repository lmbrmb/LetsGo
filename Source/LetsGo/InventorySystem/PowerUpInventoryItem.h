#pragma once

#include "InventoryItem.h"

///<summary>
/// Powerup inventory item
///</summary>
class LETSGO_API PowerUpInventoryItem final : public InventoryItem
{
public:
	explicit PowerUpInventoryItem(FName id);
	
	~PowerUpInventoryItem();
};
