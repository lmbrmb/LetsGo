#include "FirstBloodMatchEventProcessor.h"

FMatchHighlight FirstBloodMatchEventProcessor::ProcessMatchEvent(const MatchEvent& matchEvent)
{
	if(_isHappened)
	{
		return FMatchHighlight::None;
	}

	auto const isFirstBlood = matchEvent.DamagedPlayerHealth <= 0;

	if(isFirstBlood)
	{
		_isHappened = true;
	}
	
	return isFirstBlood ? FMatchHighlight::FirstBlood : FMatchHighlight::None;
}
