#include "ExcellentMatchEventProcessor.h"

const float ExcellentMatchEventProcessor::UNDEFINED_TIME = -1.0f;

const float ExcellentMatchEventProcessor::REQUIRED_TIME_INTERVAL = 3.0f;

FMatchHighlight ExcellentMatchEventProcessor::ProcessMatchEvent(const MatchEvent& matchEvent)
{
	auto highlight = FMatchHighlight::None;
	auto const isDead = matchEvent.DamagedPlayerHealth <= 0;
	if(!isDead)
	{
		return highlight;
	}

	//Reset fragged player frag time
	auto const fraggedPlayerId = matchEvent.DamagedPlayerId;
	if(_playerLastFragTime.Contains(fraggedPlayerId))
	{
		_playerLastFragTime[fraggedPlayerId] = UNDEFINED_TIME;
	}
	else
	{
		_playerLastFragTime.Add(fraggedPlayerId, UNDEFINED_TIME);
	}
	
	auto const instigatorPlayerId = matchEvent.InstigatorPlayerId;

	//Update instigator player frag time
	auto const matchEventTime = matchEvent.Time;
	if (_playerLastFragTime.Contains(instigatorPlayerId))
	{
		auto const lastFragTime = _playerLastFragTime[instigatorPlayerId];
		
		if( lastFragTime != UNDEFINED_TIME
			&& matchEventTime - lastFragTime <= REQUIRED_TIME_INTERVAL
		)
		{
			highlight = FMatchHighlight::Excellent;
			_playerLastFragTime[instigatorPlayerId] = UNDEFINED_TIME;
		}
		else
		{
			_playerLastFragTime[instigatorPlayerId] = matchEventTime;
		}
	}
	else
	{
		_playerLastFragTime.Add(instigatorPlayerId, matchEventTime);
	}

	return highlight;
}
