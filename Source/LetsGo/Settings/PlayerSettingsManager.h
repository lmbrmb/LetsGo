#pragma once
#include "PlayerSetting.h"
#include "LetsGo/Utils/AssertUtils.h"

class PlayerSettingsManager
{
public:
	PlayerSettingsManager() = default;

	void AddSetting(const FName& id, IPlayerSetting* playerSetting);

	template<class T>
	T GetSetting(const FName& id);

	template<class T>
	void SetSetting(const FName& id, T value);

private:
	TMap<FName, IPlayerSetting*> _playerSettings;
};

template <class T>
T PlayerSettingsManager::GetSetting(const FName& id)
{
	AssertContainerContainsElement(_playerSettings, id, T());
	auto setting = _playerSettings[id];
	auto const specificSetting = dynamic_cast<PlayerSetting<T>*>(setting);
	return specificSetting->GetValue();
}

template <class T>
void PlayerSettingsManager::SetSetting(const FName& id, T value)
{
	AssertContainerContainsElement(_playerSettings, id);
	auto setting = _playerSettings[id];
	auto const specificSetting = dynamic_cast<PlayerSetting<T>*>(setting);
	specificSetting->SetValue(value);
}
