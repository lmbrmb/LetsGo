#include "ExcellentMedalProcessor.h"

const float ExcellentMedalProcessor::UNDEFINED_TIME = -1.0f;

const float ExcellentMedalProcessor::REQUIRED_TIME_INTERVAL = 3.0f;

bool ExcellentMedalProcessor::ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal)
{
	auto const damagedPlayerIsDead = damageEvent.DamagedPlayerHealth <= 0;
	
	if(!damagedPlayerIsDead)
	{
		return false;
	}

	//Reset fragged player frag time
	auto const fraggedPlayerId = damageEvent.DamagedPlayerId;
	if(_playerLastFragTime.Contains(fraggedPlayerId))
	{
		_playerLastFragTime[fraggedPlayerId] = UNDEFINED_TIME;
	}
	else
	{
		_playerLastFragTime.Add(fraggedPlayerId, UNDEFINED_TIME);
	}
	
	auto const instigatorPlayerId = damageEvent.InstigatorPlayerId;

	//Update instigator player frag time
	auto const damageEventTime = damageEvent.Time;
	if (_playerLastFragTime.Contains(instigatorPlayerId))
	{
		auto const lastFragTime = _playerLastFragTime[instigatorPlayerId];
		
		if( lastFragTime != UNDEFINED_TIME
			&& damageEventTime - lastFragTime <= REQUIRED_TIME_INTERVAL
		)
		{
			outMedal.PlayerId = damageEvent.InstigatorPlayerId;
			outMedal.MedalType = FMedalType::Excellent;
			_playerLastFragTime[instigatorPlayerId] = UNDEFINED_TIME;
			return true;
		}
		
		_playerLastFragTime[instigatorPlayerId] = damageEventTime;
	}
	else
	{
		_playerLastFragTime.Add(instigatorPlayerId, damageEventTime);
	}

	return false;
}
