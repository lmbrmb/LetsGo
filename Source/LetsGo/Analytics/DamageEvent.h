#pragma once

#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponType.h"

/// <summary>
/// Analytics damage event. Immutable
/// </summary>
struct DamageEvent
{
public:
	DamageEvent(
		const float time,
		const PlayerId& instigatorPlayerId,
		const WeaponType& instigatorWeaponType,
		const PlayerId& damagedPlayerId,
		const float damagedPlayerHealth
	);

	float GetTime() const;

	const PlayerId& GetInstigatorPlayerId() const;

	const WeaponType& GetInstigatorWeaponType() const;

	const PlayerId& GetDamagedPlayerId() const;

	float GetDamagedPlayerHealth() const;

private:
	float _time;
	
	PlayerId _instigatorPlayerId;

	WeaponType _instigatorWeaponType;
	
	PlayerId _damagedPlayerId;

	float _damagedPlayerHealth;
};
