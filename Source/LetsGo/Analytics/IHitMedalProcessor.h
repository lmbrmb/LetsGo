#pragma once

#include "HitEvent.h"
#include "Medal.h"

/// <summary>
/// [Interface] Hit medal processor. Processes hit event and checks if medal was earned. Has state
/// </summary>
class IHitMedalProcessor
{
public:
	virtual ~IHitMedalProcessor() = default;
	
	virtual bool ProcessHitEvent(const HitEvent& hitEvent, Medal& outMedal) = 0;
};
