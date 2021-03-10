#pragma once

struct MatchEvent
{
public:
	MatchEvent(
		const float time,
		const int32 instigatorPlayerId,
		const int32 damagedPlayerId,
		const float damagedPlayerHealth
	):
		Time(time),
		InstigatorPlayerId(instigatorPlayerId),
		DamagedPlayerId(damagedPlayerId),
		DamagedPlayerHealth(damagedPlayerHealth)
	{
	};

	const float Time;
	
	const int32 InstigatorPlayerId;

	const int32 DamagedPlayerId;

	const float DamagedPlayerHealth;
};
