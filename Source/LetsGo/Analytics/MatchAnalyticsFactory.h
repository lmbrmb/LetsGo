#pragma once

#include "MatchAnalytics.h"

class MatchAnalyticsFactory
{
public:
	MatchAnalyticsFactory() = default;

	MatchAnalytics* Create(AMatchGameMode* matchGameMode);
};

Expose_TNameOf(MatchAnalyticsFactory)
