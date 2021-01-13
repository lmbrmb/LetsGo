#pragma once

////<summary>
///Inventory item
///</summary>
class LETSGO_API InventoryItem
{
public:
	explicit InventoryItem(FString* id);
	~InventoryItem();

private:
	FString* _id;
};
