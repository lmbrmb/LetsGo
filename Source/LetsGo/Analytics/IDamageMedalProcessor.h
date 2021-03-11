#pragma once

#include "DamageEvent.h"
#include "Medal.h"

/// <summary>
/// [Interface] Damage medal processor. Processes damage event and checks if medal was earned. Has state
/// </summary>
class IDamageMedalProcessor
{
public:
	virtual ~IDamageMedalProcessor() = default;
	
	virtual bool ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal) = 0;
};
