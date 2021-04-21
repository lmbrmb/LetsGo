#include "PlayerSettingsManager.h"

void PlayerSettingsManager::AddSetting(const FName& id, IPlayerSetting* playerSetting)
{
	_playerSettings.Add(id, playerSetting);
}
