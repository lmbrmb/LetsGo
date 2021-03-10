#include "FirstBloodMatchEventProcessor.h"

bool FirstBloodMatchEventProcessor::IsOneTimeOnly()
{
	return true;
}

bool FirstBloodMatchEventProcessor::TryProcessMatchEvent(const MatchEvent& matchEvent)
{
	return matchEvent.DamagedPlayerHealth <= 0;
}

FMatchHighlight FirstBloodMatchEventProcessor::GetHighlight() const
{
	return FMatchHighlight::FirstBlood;
}
