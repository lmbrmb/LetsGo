#pragma once

#include "Item.h"

///<summary>
/// Ammo item
///</summary>
class LETSGO_API AmmoItem final : public Item
{
public:
	explicit AmmoItem(
		const FName id,
		const int quantity,
		const int maxQuantity
	);

	int GetQuantity() const;

	int GetMaxQuantity() const;
	
private:
	int _quantity;

	int _maxQuantity;
};
