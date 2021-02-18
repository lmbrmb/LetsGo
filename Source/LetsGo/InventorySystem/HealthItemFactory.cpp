#include "HealthItemFactory.h"
#include "HealthItem.h"

HealthItemFactory::HealthItemFactory()
{
	_healAmounts.Add("MajorHealth", 50);
	_healAmounts.Add("MinorHealth", 25);
}

HealthItemFactory::~HealthItemFactory()
{
	//Do nothing
}

InventoryItem* HealthItemFactory::Create(FName itemId)
{
	if(_healAmounts.Contains(itemId))
	{
		auto const healAmount = _healAmounts[itemId];
		return new HealthItem(itemId, true, healAmount);
	}
	
	return nullptr;
}
