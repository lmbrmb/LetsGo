#pragma once
#include "Item.h"

/// <summary>
/// [Interface] Item processor
/// </summary>
class IItemProcessor
{
public:
	virtual bool TryProcessItem(Item* item) = 0;
};