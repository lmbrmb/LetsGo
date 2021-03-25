#include "Damage.h"

Damage::Damage():
	_amount(0)
{
}

Damage::Damage(const float amount):
	_amount(amount)
{
}

Damage::Damage(
	const PlayerId& instigatorPlayerId,
	const WeaponType& instigatorWeaponType,
	const float amount
) :
	_instigatorPlayerId(instigatorPlayerId),
	_instigatorWeaponType(instigatorWeaponType),
	_amount(amount)
{
}

const PlayerId& Damage::GetInstigatorPlayerId() const
{
	return _instigatorPlayerId;
}

const WeaponType& Damage::GetInstigatorWeaponType() const
{
	return _instigatorWeaponType;
}

float Damage::GetAmount() const
{
	return _amount;
}
