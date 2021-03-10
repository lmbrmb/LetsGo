#pragma once

#include "IMatchEventProcessor.h"

/// <summary>
/// Excellent match event processor.
/// Conditions: Player got 2 frags in 3 seconds. Frag count resets on player death
/// </summary>
class ExcellentMatchEventProcessor final : public IMatchEventProcessor
{
public:
	ExcellentMatchEventProcessor() = default;
	
	virtual FMatchHighlight ProcessMatchEvent(const MatchEvent& matchEvent) override;

private:
	static const float REQUIRED_TIME_INTERVAL;
	
	static const float UNDEFINED_TIME;
	
	TMap<int32, float> _playerLastFragTime;
};
