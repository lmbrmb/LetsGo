#pragma once

////<summary>
/// [Abstract] Inventory item contract
///</summary>
class LETSGO_API InventoryItem
{
public:
	explicit InventoryItem(
		const FName& id,
		const bool isConsumable, 
		const bool isSingleInstance,
		const int quantity,
		const int maxQuantity
	);
	
	virtual ~InventoryItem();
	
	FName GetId() const;

	bool IsConsumable() const;

	bool IsSingleInstance() const;

	/// <summary>
	/// Increases quantity. Can't add more than max quantity.
	/// </summary>
	/// <param name="amount">How much to add</param>
	/// <returns>Added items quantity</returns>
	int IncreaseQuantity(const int amount);

	/// <summary>
	/// Decreases quantity. Can't remove more than current quantity.
	/// </summary>
	/// <param name="amount">How much to add</param>
	/// <returns>Removed items quantity</returns>
	int DecreaseQuantity(const int amount);
	
	int GetQuantity() const;
	
	int GetMaxQuantity() const;
	
private:
	FName _id;

	bool _isConsumable;

	bool _isSingleInstance;

	int _quantity;
	
	int _maxQuantity;
};
