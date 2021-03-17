#pragma once

#include "LetsGo/Data/PlayerId.h"

/// <summary>
/// Analytics hit event. Immutable
/// </summary>
struct HitEvent
{
public:
	HitEvent(
		const float time,
		const bool isHittedPlayer,
		const PlayerId instigatorPlayerId,
		const FName& instigatorWeaponId
	);

	float GetTime() const;

	bool GetIsHittedPlayer() const;

	const PlayerId& GetInstigatorPlayerId() const;

	const FName& GetInstigatorWeaponId() const;

private:
	const float _time;

	const bool _isHittedPlayer;
	
	const PlayerId _instigatorPlayerId;

	const FName _instigatorWeaponId;
};
