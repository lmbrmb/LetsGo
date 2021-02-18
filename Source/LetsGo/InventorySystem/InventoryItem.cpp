#include "InventoryItem.h"

InventoryItem::InventoryItem(FName id, bool isConsumable) :
	_id(id),
	_isConsumable(isConsumable)
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
