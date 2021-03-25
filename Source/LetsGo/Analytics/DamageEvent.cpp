#include "DamageEvent.h"

DamageEvent::DamageEvent(
	const float time,
	const PlayerId& instigatorPlayerId,
	const WeaponType& instigatorWeaponType,
	const PlayerId& damagedPlayerId,
	const float damagedPlayerHealth
) :
	_time(time),
	_instigatorPlayerId(instigatorPlayerId),
	_instigatorWeaponType(instigatorWeaponType),
	_damagedPlayerId(damagedPlayerId),
	_damagedPlayerHealth(damagedPlayerHealth)
{
};

float DamageEvent::GetTime() const
{
	return _time;
}

const PlayerId& DamageEvent::GetInstigatorPlayerId() const
{
	return _instigatorPlayerId;
}

const WeaponType& DamageEvent::GetInstigatorWeaponType() const
{
	return _instigatorWeaponType;
}

const PlayerId& DamageEvent::GetDamagedPlayerId() const
{
	return _damagedPlayerId;
}

float DamageEvent::GetDamagedPlayerHealth() const
{
	return _damagedPlayerHealth;
}