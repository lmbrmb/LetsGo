#pragma once

#include "InventoryItemFactory.h"

////<summary>
///Inventory item
///</summary>
class LETSGO_API HealthItemFactory final : public InventoryItemFactory
{
public:
	HealthItemFactory();
	
	~HealthItemFactory();
	
	virtual InventoryItem* Create(FName itemId) override;

private:
	TMap<FName, float> _healAmounts;
};

Expose_TNameOf(HealthItemFactory)
