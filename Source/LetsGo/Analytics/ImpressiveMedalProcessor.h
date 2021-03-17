#pragma once

#include "IDamageMedalProcessor.h"
#include "IHitMedalProcessor.h"
#include "LetsGo/Data/WeaponId.h"

/// <summary>
/// Impressive medal processor.
/// Conditions: Player got 2 consequent railgun hits. Hit count resets on player death
/// </summary>
class ImpressiveMedalProcessor final : public IDamageMedalProcessor, public IHitMedalProcessor
{
public:
	ImpressiveMedalProcessor(const int requiredHitCount, const WeaponId& weaponId);
	
	virtual bool ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal) override;

	virtual bool ProcessHitEvent(const HitEvent& hitEvent, Medal& outMedal) override;

private:
	int _requiredHitCount;

	WeaponId _weaponId;
	
	TMap<int, int> _playerRailgunHitCount;
};
