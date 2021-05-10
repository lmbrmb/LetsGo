#include "PlayerSettingsManager.h"

void PlayerSettingsManager::SetDefaultValues(UPlayerSettings* settings) const
{
	settings->ShouldEquipWeaponOnPickup = true;
	settings->CenterWeapon = false;
	settings->PlayerSkinId = "Vampire";
	settings->MouseSensitivity = 5;
}
