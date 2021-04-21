#include "PlayerSettingsManagerFactory.h"

#include "PlayerSetting.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
PlayerSettingsManager* PlayerSettingsManagerFactory::Create()
{
	auto const playerSettingsManager = new PlayerSettingsManager();

	auto const weaponPivotSetting = new PlayerSetting<int>(0);

	playerSettingsManager->AddSetting("WeaponPivotIndex", weaponPivotSetting);
	return playerSettingsManager;
}
