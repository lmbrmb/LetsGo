#include "HitEvent.h"

HitEvent::HitEvent(
	const float time,
	const bool isHittedPlayer,
	const PlayerId& instigatorPlayerId,
	const WeaponId& instigatorWeaponId
) :
	_time(time),
	_isHittedPlayer(isHittedPlayer),
	_instigatorPlayerId(instigatorPlayerId),
	_instigatorWeaponId(instigatorWeaponId)
{
}

float HitEvent::GetTime() const
{
	return _time;
}

bool HitEvent::GetIsHittedPlayer() const
{
	return _isHittedPlayer;
}

const PlayerId& HitEvent::GetInstigatorPlayerId() const
{
	return _instigatorPlayerId;
}

const WeaponId& HitEvent::GetInstigatorWeaponId() const
{
	return _instigatorWeaponId;
};