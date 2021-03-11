#pragma once

struct DamageEvent
{
public:
	DamageEvent(
		const float time,
		const int32 instigatorPlayerId,
		const FName& instigatorWeaponId,
		const int32 damagedPlayerId,
		const float damagedPlayerHealth
	):
		Time(time),
		InstigatorPlayerId(instigatorPlayerId),
		InstigatorWeaponId(instigatorWeaponId),
		DamagedPlayerId(damagedPlayerId),
		DamagedPlayerHealth(damagedPlayerHealth)
	{
	};

	const float Time;
	
	const int32 InstigatorPlayerId;

	const FName InstigatorWeaponId;
	
	const int32 DamagedPlayerId;

	const float DamagedPlayerHealth;
};
