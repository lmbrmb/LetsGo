#pragma once

#include "LetsGo/Data/PlayerId.h"

/// <summary>
/// Analytics damage event. Immutable
/// </summary>
struct DamageEvent
{
public:
	DamageEvent(
		const float time,
		const PlayerId& instigatorPlayerId,
		const FName& instigatorWeaponId,
		const PlayerId& damagedPlayerId,
		const float damagedPlayerHealth
	);

	float GetTime() const;

	const PlayerId& GetInstigatorPlayerId() const;

	const FName& GetInstigatorWeaponId() const;

	const PlayerId& GetDamagedPlayerId() const;

	float GetDamagedPlayerHealth() const;

private:
	float _time;
	
	PlayerId _instigatorPlayerId;

	FName _instigatorWeaponId;
	
	PlayerId _damagedPlayerId;

	float _damagedPlayerHealth;
};
