#include "FirstBloodMedalProcessor.h"

bool FirstBloodMedalProcessor::ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal)
{
	if(_isHappened)
	{
		return false;
	}

	auto const isDead = damageEvent.GetDamagedPlayerHealth() <= 0;

	if(!isDead)
	{
		return false;
	}
	
	_isHappened = true;

	const Medal firstBloodMedal(damageEvent.GetInstigatorPlayerId(), FMedalType::FirstBlood);
	outMedal = firstBloodMedal;
	
	return true;
}
