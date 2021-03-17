#include "ExcellentMedalProcessor.h"

const float ExcellentMedalProcessor::UNDEFINED_TIME = -1.0f;

ExcellentMedalProcessor::ExcellentMedalProcessor(const float requiredTimeInterval) :
	_requiredTimeInterval(requiredTimeInterval)
{
}

bool ExcellentMedalProcessor::ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal)
{
	auto const damagedPlayerIsDead = damageEvent.GetDamagedPlayerHealth() <= 0;
	
	if(!damagedPlayerIsDead)
	{
		return false;
	}

	//Reset fragged player frag time
	auto const fraggedPlayerIdValue = damageEvent.GetDamagedPlayerId().GetId();
	if(_playerLastFragTime.Contains(fraggedPlayerIdValue))
	{
		_playerLastFragTime[fraggedPlayerIdValue] = UNDEFINED_TIME;
	}
	else
	{
		_playerLastFragTime.Add(fraggedPlayerIdValue, UNDEFINED_TIME);
	}

	if(damageEvent.GetInstigatorPlayerId() == damageEvent.GetDamagedPlayerId())
	{
		// Self harm
		return false;
	}
	
	auto const instigatorPlayerIdValue = damageEvent.GetInstigatorPlayerId().GetId();

	//Update instigator player frag time
	auto const damageEventTime = damageEvent.GetTime();
	if (_playerLastFragTime.Contains(instigatorPlayerIdValue))
	{
		auto const lastFragTime = _playerLastFragTime[instigatorPlayerIdValue];
		
		if( lastFragTime != UNDEFINED_TIME
			&& damageEventTime - lastFragTime <= _requiredTimeInterval
		)
		{
			const Medal excellentMedal(damageEvent.GetInstigatorPlayerId(), FMedalType::Excellent);
			outMedal = excellentMedal;
			_playerLastFragTime[instigatorPlayerIdValue] = UNDEFINED_TIME;
			return true;
		}
		
		_playerLastFragTime[instigatorPlayerIdValue] = damageEventTime;
	}
	else
	{
		_playerLastFragTime.Add(instigatorPlayerIdValue, damageEventTime);
	}

	return false;
}
