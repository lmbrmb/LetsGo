#pragma once

#include "IDamageMedalProcessor.h"

/// <summary>
/// Excellent medal processor.
/// Conditions: Player got 2 frags in 3 seconds. Frag count resets on player death
/// </summary>
class ExcellentMedalProcessor final : public IDamageMedalProcessor
{
public:
	ExcellentMedalProcessor() = default;
	
	virtual bool ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal) override;

private:
	static const float REQUIRED_TIME_INTERVAL;
	
	static const float UNDEFINED_TIME;
	
	TMap<int32, float> _playerLastFragTime;
};
