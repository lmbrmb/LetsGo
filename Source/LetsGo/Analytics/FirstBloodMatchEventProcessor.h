#pragma once
#include "IMatchEventProcessor.h"

/// <summary>
/// One time
/// </summary>
class FirstBloodMatchEventProcessor final : public IMatchEventProcessor
{
public:
	FirstBloodMatchEventProcessor() = default;

	virtual bool IsOneTimeOnly() override;
	
	virtual bool TryProcessMatchEvent(const MatchEvent& matchEvent) override;

	virtual FMatchHighlight GetHighlight() const override;

private:
	bool _isFirstBloodProcessed = false;
};
