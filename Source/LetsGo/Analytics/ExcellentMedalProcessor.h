#pragma once

#include "IDamageMedalProcessor.h"

/// <summary>
/// Excellent medal processor.
/// Conditions: Player got 2 frags in N seconds. Frag count resets on player death
/// </summary>
class ExcellentMedalProcessor final : public IDamageMedalProcessor
{
public:
	ExcellentMedalProcessor(float requiredTimeInterval);
	
	virtual bool ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal) override;

private:
	float _requiredTimeInterval;
	
	static const float UNDEFINED_TIME;
	
	TMap<int, float> _playerLastFragTime;
};
