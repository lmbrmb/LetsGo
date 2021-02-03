#pragma once

////<summary>
/// [Abstract] Inventory item
///</summary>
class LETSGO_API InventoryItem
{
public:
	explicit InventoryItem(FName id);
	
	~InventoryItem();

	FName GetId() const
	{
		return _id;
	}
	
private:
	FName _id;
};
