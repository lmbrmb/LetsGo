#include "MatchSettingsManager.h"

void MatchSettingsManager::SetDefaultValues(UMatchSettings* settings) const
{
	settings->BotCount = 3;
	settings->FragLimit = 20;
}
