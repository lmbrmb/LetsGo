#pragma once

struct MatchEvent
{
public:
	MatchEvent(
		const float time,
		const FGuid instigatorPlayerId,
		const FGuid damagedPlayerId,
		const float damagedPlayerHealth
	):
		Time(time),
		InstigatorPlayerId(instigatorPlayerId),
		DamagedPlayerId(damagedPlayerId),
		DamagedPlayerHealth(damagedPlayerHealth)
	{
	};

	const float Time;
	
	const FGuid InstigatorPlayerId;

	const FGuid DamagedPlayerId;

	const float DamagedPlayerHealth;
};
