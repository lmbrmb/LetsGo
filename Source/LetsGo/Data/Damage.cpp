#include "Damage.h"

Damage::Damage():
	_weaponId(""),
	_amount(0)
{
}

Damage::Damage(const float amount):
	_weaponId(""),
	_amount(amount)
{
}

Damage::Damage(
	const PlayerId& instigatorId,
	const FName& weaponId,
	const float amount
) :
	_instigatorId(instigatorId),
	_weaponId(weaponId),
	_amount(amount)
{
}

const PlayerId& Damage::GetInstigatorId() const
{
	return _instigatorId;
}

const FName& Damage::GetWeaponId() const
{
	return _weaponId;
}

float Damage::GetAmount() const
{
	return _amount;
}
