#include "MatchAnalyticsFactory.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
MatchAnalytics* MatchAnalyticsFactory::Create(AMatchGameMode* matchGameMode)
{
	return new MatchAnalytics(matchGameMode);
}
