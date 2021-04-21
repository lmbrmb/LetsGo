#include "Damage.h"

Damage::Damage():
	_amount(0),
	_isHitted(false)
{
}

Damage::Damage(const float amount):
	_amount(amount),
	_isHitted(false)
{
}

Damage::Damage(
	const PlayerId& instigatorPlayerId,
	const WeaponType& instigatorWeaponType,
	const FHitResult& hitResult,
	const float amount
) :
	_instigatorPlayerId(instigatorPlayerId),
	_instigatorWeaponType(instigatorWeaponType),
	_hitResult(hitResult),
	_amount(amount),
	_isHitted(true)
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

const FHitResult& Damage::GetHitResult() const
{
	return _hitResult;
}

float Damage::GetAmount() const
{
	return _amount;
}

bool Damage::GetIsHitted() const
{
	return _isHitted;
}
