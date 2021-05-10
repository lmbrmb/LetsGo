#pragma once

#include "PlayerSettingsManager.h"

/// <summary>
/// Settings manager factory
/// </summary>
class SettingsManagerFactory final
{
public:
	// ReSharper disable once CppNonExplicitConvertingConstructor
	SettingsManagerFactory(IUObjectRegistry* uObjectRegistry);
	
	template<class TSettingsManager>
	TSettingsManager* Create(
		const FString& slotName,
		const bool isLazyInitialization
	) const;

private:
	IUObjectRegistry* _uObjectRegistry = nullptr;
};

template <class TSettingsManager>
TSettingsManager* SettingsManagerFactory::Create(
	const FString& slotName,
	const bool isLazyInitialization) const
{
	auto const playerSettingsManager = new TSettingsManager(slotName, 0, _uObjectRegistry, isLazyInitialization);
	return playerSettingsManager;
}

Expose_TNameOf(SettingsManagerFactory)
