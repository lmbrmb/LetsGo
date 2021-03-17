#include "DamageEvent.h"

DamageEvent::DamageEvent(
	const float time,
	const PlayerId& instigatorPlayerId,
	const WeaponId& instigatorWeaponId,
	const PlayerId& damagedPlayerId,
	const float damagedPlayerHealth
) :
	_time(time),
	_instigatorPlayerId(instigatorPlayerId),
	_instigatorWeaponId(instigatorWeaponId),
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

const WeaponId& DamageEvent::GetInstigatorWeaponId() const
{
	return _instigatorWeaponId;
}

const PlayerId& DamageEvent::GetDamagedPlayerId() const
{
	return _damagedPlayerId;
}

float DamageEvent::GetDamagedPlayerHealth() const
{
	return _damagedPlayerHealth;
}