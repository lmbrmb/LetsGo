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
	const WeaponId& instigatorWeaponId,
	const float amount
) :
	_instigatorPlayerId(instigatorPlayerId),
	_instigatorWeaponId(instigatorWeaponId),
	_amount(amount)
{
}

const PlayerId& Damage::GetInstigatorPlayerId() const
{
	return _instigatorPlayerId;
}

const WeaponId& Damage::GetInstigatorWeaponId() const
{
	return _instigatorWeaponId;
}

float Damage::GetAmount() const
{
	return _amount;
}
