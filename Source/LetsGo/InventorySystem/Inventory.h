#pragma once

#include "InventoryItem.h"

///<summary>
///Inventory
///</summary>
class LETSGO_API Inventory
{
public:
	Inventory();
	
	~Inventory();

	void AddItem(FString* itemId);

	void RemoveItem(FString* itemId);

	// TODO: Items collection
};
