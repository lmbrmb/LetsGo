#include "Damage.h"

Damage::Damage(const FGuid instigatorId, const float amount) :
_instigatorId(instigatorId),
_amount(amount)
{
}

FGuid Damage::GetInstigatorId() const
{
	return _instigatorId;
}

float Damage::GetAmount() const
{
	return _amount;
}
