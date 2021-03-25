#pragma once

#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponType.h"

/// <summary>
/// Analytics hit event. Immutable
/// </summary>
struct HitEvent
{
public:
	HitEvent(
		const float time,
		const bool isHittedPlayer,
		const PlayerId& instigatorPlayerId,
		const WeaponType& instigatorWeaponType
	);

	float GetTime() const;

	bool GetIsHittedPlayer() const;

	const PlayerId& GetInstigatorPlayerId() const;

	const WeaponType& GetInstigatorWeaponType() const;

private:
	const float _time;

	const bool _isHittedPlayer;
	
	const PlayerId _instigatorPlayerId;

	const WeaponType _instigatorWeaponType;
};
