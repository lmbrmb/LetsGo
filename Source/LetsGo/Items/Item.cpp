#include "Item.h"

Item::Item(const FName id) :
	_id(id)
{
	//Do nothing
}

FName Item::GetId() const
{
	return _id;
}
