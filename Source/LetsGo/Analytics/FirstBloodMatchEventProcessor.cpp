#include "FirstBloodMatchEventProcessor.h"

FMatchHighlight FirstBloodMatchEventProcessor::ProcessMatchEvent(const MatchEvent& matchEvent)
{
	if(_isHappened)
	{
		return FMatchHighlight::None;
	}

	auto const isDead = matchEvent.DamagedPlayerHealth <= 0;

	if(isDead)
	{
		_isHappened = true;
	}
	
	return isDead ? FMatchHighlight::FirstBlood : FMatchHighlight::None;
}
