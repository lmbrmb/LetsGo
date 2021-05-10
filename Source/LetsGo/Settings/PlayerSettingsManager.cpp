#include "PlayerSettingsManager.h"

void PlayerSettingsManager::SetDefaultValues(UPlayerSettings* settings) const
{
	settings->ShouldEquipWeaponOnPickup = true;
	settings->WeaponPivotIndex = 0;
	settings->PlayerSkinId = "Vampire";
	settings->MouseSensitivity = 5;
}
