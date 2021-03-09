#include "Item.h"

Item::Item(const FName id) :
	_id(id)
{
}

FName Item::GetId() const
{
	return _id;
}
