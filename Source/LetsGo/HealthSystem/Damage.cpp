#include "Damage.h"

Damage::Damage(const int32 instigatorId, const float amount) :
_instigatorId(instigatorId),
_amount(amount)
{
}

int32 Damage::GetInstigatorId() const
{
	return _instigatorId;
}

float Damage::GetAmount() const
{
	return _amount;
}
