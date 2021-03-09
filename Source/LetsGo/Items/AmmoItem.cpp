#include "AmmoItem.h"

AmmoItem::AmmoItem(
	const FName id,
	const int quantity,
	const int maxQuantity
)
	: Item(id),
	_quantity(quantity),
	_maxQuantity(maxQuantity)
{
}

int AmmoItem::GetQuantity() const
{
	return _quantity;
}

int AmmoItem::GetMaxQuantity() const
{
	return _maxQuantity;
}
