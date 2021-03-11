#pragma once

#include "IDamageMedalProcessor.h"

/// <summary>
/// Impressive medal processor.
/// Conditions: Player got 2 consequent railgun hits. Hit count resets on player death
/// </summary>
class ImpressiveMedalProcessor final : public IDamageMedalProcessor
{
public:
	ImpressiveMedalProcessor() = default;
	
	virtual bool ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal) override;

private:
	static const int REQUIRED_HIT_COUNT;

	static const FName RAILGUN_ID;
	
	TMap<int32, int> _playerRailgunHitCount;
};
