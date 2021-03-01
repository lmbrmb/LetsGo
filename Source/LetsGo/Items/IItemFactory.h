#pragma once

#include "Item.h"

////<summary>
/// [Interface] Item factory contract
///</summary>
class IItemFactory
{
public:
	IItemFactory() = default;
	
	virtual ~IItemFactory() = default;

	virtual Item* Get(FName itemId) = 0;
};