#pragma once

#include "Kismet/GameplayStatics.h"
#include "LetsGo/Data/IUObjectRegistry.h"
#include "LetsGo/Utils/AssertUtils.h"

template<class TSettings>
class SettingsManager
{
public:
	// ReSharper disable once CppNonExplicitConvertingConstructor
	SettingsManager(
		const FString& slotName,
		const int userIndex,
		IUObjectRegistry* uObjectRegistry,
		const bool isLazyInitialization
	);

	virtual ~SettingsManager() = default;
	
	/// <summary>
	/// Returns cached settings. Loads settings if cache is empty
	/// </summary>
	/// <returns></returns>
	TSettings* GetSettings();

	/// <summary>
	/// Saves cached settings
	/// </summary>
	void Save();

	/// <summary>
	/// If settings exist: Load / Reload, Create otherwise. Caches result
	/// </summary>
	TSettings* LoadOrCreate();

protected:
	virtual void SetDefaultValues(TSettings* settings) const = 0;

private:
	FString _slotName;

	int _userIndex;

	IUObjectRegistry* _uObjectRegistry = nullptr;
	
	TSettings* _settingsCached = nullptr;

	TSettings* Load() const;

	TSettings* Create() const;
};

template <class TSettings>
SettingsManager<TSettings>::SettingsManager(
	const FString& slotName,
	const int userIndex,
	IUObjectRegistry* uObjectRegistry,
	const bool isLazyInitialization
) :
	_slotName(slotName),
	_userIndex(userIndex),
	_uObjectRegistry(uObjectRegistry)
{
	if (!isLazyInitialization)
	{
		LoadOrCreate();
	}
}

template <class TSettings>
TSettings* SettingsManager<TSettings>::GetSettings()
{
	if (!_settingsCached)
	{
		return LoadOrCreate();
	}

	return _settingsCached;
}

template <class TSettings>
void SettingsManager<TSettings>::Save()
{
	AssertIsNotNull(_settingsCached);
	UGameplayStatics::SaveGameToSlot(_settingsCached, _slotName, _userIndex);
}

template <class TSettings>
TSettings* SettingsManager<TSettings>::LoadOrCreate()
{
	if (_settingsCached)
	{
		_uObjectRegistry->UnRegisterUObject(_settingsCached);
	}
	
	_settingsCached = Load();

	if (!_settingsCached)
	{
		_settingsCached = Create();
	}

	AssertIsNotNull(_settingsCached, nullptr);
	_uObjectRegistry->RegisterUObject(_settingsCached);
	
	return _settingsCached;
}

template <class TSettings>
TSettings* SettingsManager<TSettings>::Load() const
{
	auto const loadedSaveGame = UGameplayStatics::LoadGameFromSlot(_slotName, _userIndex);
	auto const settings = Cast<TSettings>(loadedSaveGame);
	return settings;
}

template <class TSettings>
TSettings* SettingsManager<TSettings>::Create() const
{
	auto const createdSaveGame = UGameplayStatics::CreateSaveGameObject(TSettings::StaticClass());
	auto const settings = Cast<TSettings>(createdSaveGame);
	SetDefaultValues(settings);

	return settings;
}
