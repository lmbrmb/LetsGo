#pragma once

#include "HealthItem.h"
#include "ItemFactory.h"

////<summary>
///Health item
///</summary>
class LETSGO_API HealthItemFactory final : public ItemFactory<HealthItem>
{
class HealthData
{
public:
	explicit HealthData(float healAmount) :
	HealAmount(healAmount)
	{
		//Do nothing
	}

	float HealAmount;
};
	
public:
	HealthItemFactory();

protected:
	virtual HealthItem* CreateItem(const FName itemId) override;
	
private:
	TMap<FName, HealthData*> _healthData;
};

Expose_TNameOf(HealthItemFactory)
