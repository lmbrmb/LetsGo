#include "HitEvent.h"

HitEvent::HitEvent(
	const float time,
	const bool isHittedPlayer,
	const PlayerId& instigatorPlayerId,
	const WeaponType& instigatorWeaponType
) :
	_time(time),
	_isHittedPlayer(isHittedPlayer),
	_instigatorPlayerId(instigatorPlayerId),
	_instigatorWeaponType(instigatorWeaponType)
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

const WeaponType& HitEvent::GetInstigatorWeaponType() const
{
	return _instigatorWeaponType;
};
