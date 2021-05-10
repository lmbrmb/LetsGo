#pragma once

#include "MatchSettings.h"
#include "SettingsManager.h"

class MatchSettingsManager final : public SettingsManager<UMatchSettings>
{
public:
	MatchSettingsManager(
		const FString& slotName,
		const int userIndex,
		IUObjectRegistry* uObjectRegistry,
		const bool isLazyInitialization
	)
		: SettingsManager<UMatchSettings>(slotName, userIndex, uObjectRegistry, isLazyInitialization)
	{
	}

protected:
	virtual void SetDefaultValues(UMatchSettings* settings) const override;
};
