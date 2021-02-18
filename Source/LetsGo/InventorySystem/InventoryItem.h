#pragma once

////<summary>
/// [Abstract] Inventory item contract
///</summary>
class LETSGO_API InventoryItem
{
public:
	explicit InventoryItem(FName id, bool isConsumable);
	
	virtual ~InventoryItem();
	
	FName GetId() const;

	bool IsConsumable() const;
	
private:
	FName _id;

	bool _isConsumable;
};
