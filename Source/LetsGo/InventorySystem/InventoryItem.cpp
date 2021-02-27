#include "InventoryItem.h"

InventoryItem::InventoryItem(
	const FName& id,
	const bool isConsumable,
	const bool isSingleInstance,
	const int quantity,
	const int maxQuantity
	) :
	_id(id),
	_isConsumable(isConsumable),
	_isSingleInstance(isSingleInstance),
	_quantity(quantity),
	_maxQuantity(maxQuantity)
{
	//Do nothing
}

InventoryItem::~InventoryItem()
{
	//Do nothing
}

FName InventoryItem::GetId() const
{
	return _id;
}

bool InventoryItem::IsConsumable() const
{
	return _isConsumable;
}

bool InventoryItem::IsSingleInstance() const
{
	return _isSingleInstance;
}

int InventoryItem::IncreaseQuantity(const int amount)
{
	auto const maxAvailable = _maxQuantity - _quantity;
	auto const amountToAdd = FMath::Min(maxAvailable, amount);
	_quantity += amountToAdd;
	return amountToAdd;
}

int InventoryItem::DecreaseQuantity(const int amount)
{
	auto const amountToRemove = FMath::Min(_quantity, amount);
	_quantity -= amountToRemove;
	return amountToRemove;
}

int InventoryItem::GetQuantity() const
{
	return _quantity;
}

int InventoryItem::GetMaxQuantity() const
{
	return _maxQuantity;
}
