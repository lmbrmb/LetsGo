#pragma once

#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponId.h"

/// <summary>
/// Analytics damage event. Immutable
/// </summary>
struct DamageEvent
{
public:
	DamageEvent(
		const float time,
		const PlayerId& instigatorPlayerId,
		const WeaponId& instigatorWeaponId,
		const PlayerId& damagedPlayerId,
		const float damagedPlayerHealth
	);

	float GetTime() const;

	const PlayerId& GetInstigatorPlayerId() const;

	const WeaponId& GetInstigatorWeaponId() const;

	const PlayerId& GetDamagedPlayerId() const;

	float GetDamagedPlayerHealth() const;

private:
	float _time;
	
	PlayerId _instigatorPlayerId;

	WeaponId _instigatorWeaponId;
	
	PlayerId _damagedPlayerId;

	float _damagedPlayerHealth;
};
