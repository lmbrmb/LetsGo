#pragma once

#include "PlayerSettingsManager.h"

class PlayerSettingsManagerFactory
{
public:
	PlayerSettingsManagerFactory() = default;
	
	PlayerSettingsManager* Create();
};

Expose_TNameOf(PlayerSettingsManagerFactory)
