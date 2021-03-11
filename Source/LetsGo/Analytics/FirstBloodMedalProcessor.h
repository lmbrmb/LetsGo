#pragma once

#include "IDamageMedalProcessor.h"

/// <summary>
/// First blood medal processor.
/// Conditions: Player got the very first frag in match.
/// </summary>
class FirstBloodMedalProcessor final : public IDamageMedalProcessor
{
public:
	FirstBloodMedalProcessor() = default;
	
	virtual bool ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal) override;

private:
	bool _isHappened = false;
};
