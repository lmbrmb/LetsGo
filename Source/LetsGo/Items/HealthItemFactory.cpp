#include "HealthItemFactory.h"
#include "HealthItem.h"

HealthItemFactory::HealthItemFactory()
{
	_healthData.Add("MajorHealth", new HealthData( 50));
	_healthData.Add("MinorHealth", new HealthData(25));
}

HealthItem* HealthItemFactory::CreateItem(const FName itemId)
{
	if(!_healthData.Contains(itemId))
	{
		return nullptr;
	}
	
	auto const healthData = _healthData[itemId];
	auto const healthItem = new HealthItem(itemId, healthData->HealAmount);
	return healthItem;
}
