#pragma once

#include "IMatchEventProcessor.h"

/// <summary>
/// First blood match event processor.
/// Conditions: Player got the very first frag in match.
/// </summary>
class FirstBloodMatchEventProcessor final : public IMatchEventProcessor
{
public:
	FirstBloodMatchEventProcessor() = default;
	
	virtual FMatchHighlight ProcessMatchEvent(const MatchEvent& matchEvent) override;

private:
	bool _isHappened = false;
};
