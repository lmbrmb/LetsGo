#include "FirstBloodMedalProcessor.h"

bool FirstBloodMedalProcessor::ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal)
{
	if(_isHappened)
	{
		return false;
	}

	auto const isDead = damageEvent.DamagedPlayerHealth <= 0;

	if(!isDead)
	{
		return false;
	}
	
	_isHappened = true;

	outMedal.PlayerId = damageEvent.InstigatorPlayerId;
	outMedal.MedalType = FMedalType::FirstBlood;
	
	return true;
}
