#pragma once

#include "MatchEvent.h"
#include "MatchHighlight.h"

class IMatchEventProcessor
{
public:
	virtual ~IMatchEventProcessor() = default;
	
	virtual FMatchHighlight ProcessMatchEvent(const MatchEvent& matchEvent) = 0;
};
