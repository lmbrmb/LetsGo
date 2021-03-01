#include "HealthItem.h"

HealthItem::HealthItem(
	const FName id,
	const float healAmount
)
	: Item(id),
	_healAmount(healAmount)
{
	//Do nothing
}

int HealthItem::GetHealAmount() const
{
	return _healAmount;
}
