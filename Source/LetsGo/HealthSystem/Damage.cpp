#include "Damage.h"

Damage::Damage():
	_instigatorId(MIN_int32),
	_weaponId(""),
	_amount(0)
{
}

Damage::Damage(const float amount):
	_instigatorId(MIN_int32),
	_weaponId(""),
	_amount(amount)
{
}

Damage::Damage(
	const int32 instigatorId,
	const FName weaponId,
	const float amount
) :
	_instigatorId(instigatorId),
	_weaponId(weaponId),
	_amount(amount)
{
}

int32 Damage::GetInstigatorId() const
{
	return _instigatorId;
}

FName Damage::GetWeaponId() const
{
	return _weaponId;
}

float Damage::GetAmount() const
{
	return _amount;
}
