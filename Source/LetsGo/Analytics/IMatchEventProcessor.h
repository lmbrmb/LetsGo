#pragma once

#include "MatchEvent.h"
#include "MatchHighlight.h"

class IMatchEventProcessor
{
public:
	virtual ~IMatchEventProcessor() = default;

	virtual bool IsOneTimeOnly() = 0;
	
	virtual bool TryProcessMatchEvent(const MatchEvent& matchEvent) = 0;

	virtual FMatchHighlight GetHighlight() const = 0;
};
