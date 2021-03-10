#pragma once
#include "IMatchEventProcessor.h"

/// <summary>
/// One time
/// </summary>
class FirstBloodMatchEventProcessor final : public IMatchEventProcessor
{
public:
	FirstBloodMatchEventProcessor() = default;
	
	virtual FMatchHighlight ProcessMatchEvent(const MatchEvent& matchEvent) override;

private:
	bool _isHappened = false;
};
