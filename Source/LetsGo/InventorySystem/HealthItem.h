#pragma once

#include "InventoryItem.h"

///<summary>
/// Health item
///</summary>
class LETSGO_API HealthItem final : public InventoryItem
{
public:
	HealthItem(
		const FName& id,
		const bool isConsumable,
		const bool isSingleInstance,
		const int quantity,
		const int maxQuantity,
		const float healAmount
	);

	int GetHealAmount() const;
	
private:
	int _healAmount;
};
