#pragma once

#include "InventoryItemFactory.h"

////<summary>
///Item conversion factory
///</summary>
class LETSGO_API ItemConversionFactory final
{
public:
	ItemConversionFactory();
	
	FName GetConvertedItemId(const FName itemId);

private:
	TMap<FName, FName> _itemToItem;
};

Expose_TNameOf(ItemConversionFactory)
