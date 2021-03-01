#pragma once

#include "Item.h"

///<summary>
/// Health item
///</summary>
class LETSGO_API HealthItem final : public Item
{
public:
	HealthItem(
		const FName id,
		const float healAmount
	);

	int GetHealAmount() const;
	
private:
	int _healAmount;
};
