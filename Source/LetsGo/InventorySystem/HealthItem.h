#pragma once

#include "InventoryItem.h"

///<summary>
/// Health item
///</summary>
class LETSGO_API HealthItem final : public InventoryItem
{
public:
	explicit HealthItem(FName id, bool isConsumable, float healthAmount);

	int GetHealAmount() const;
	
private:
	int _healAmount;
};
