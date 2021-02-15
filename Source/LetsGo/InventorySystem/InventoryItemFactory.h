#pragma once

#include "InventoryItem.h"

////<summary>
/// [Abstract] Inventory item factory
///</summary>
class LETSGO_API InventoryItemFactory
{
public:
	InventoryItemFactory();
	
	virtual ~InventoryItemFactory();
	
	virtual InventoryItem* Create(FName itemId) = 0;
};

Expose_TNameOf(InventoryItemFactory)
