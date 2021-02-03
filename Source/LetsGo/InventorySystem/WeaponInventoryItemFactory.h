#pragma once

#include "InventoryItemFactory.h"

////<summary>
///Inventory item
///</summary>
class LETSGO_API WeaponInventoryItemFactory final : public InventoryItemFactory
{
public:
	WeaponInventoryItemFactory();
	
	~WeaponInventoryItemFactory();
	
	virtual InventoryItem* Create(FName itemId) override;
};
