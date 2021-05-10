#pragma once

#include "PlayerSettings.h"
#include "SettingsManager.h"

class PlayerSettingsManager final : public SettingsManager<UPlayerSettings>
{
public:
	PlayerSettingsManager(
		const FString& slotName,
		const int userIndex,
		IUObjectRegistry* uObjectRegistry,
		const bool isLazyInitialization
	)
		: SettingsManager<UPlayerSettings>(slotName, userIndex, uObjectRegistry, isLazyInitialization)
	{
	}

protected:
	virtual void SetDefaultValues(UPlayerSettings* settings) const override;
};
