#pragma once

#include "LetsGo/Logs/DevLogger.h"

/// <summary>
/// [Abstract] Loads assets, returns asset by id 
/// </summary>
/// <typeparam name="T">Asset type</typeparam>
template <class T>
class CachedAssetFactory
{
public:
	T* GetOrLoad(const FName id);

protected:
	CachedAssetFactory() = default;

	TMap<const FName, FString> Paths;

	void LoadAllAssets();

private:
	TMap<const FName, T*> Loaded;
};

template <class T>
T* CachedAssetFactory<T>::GetOrLoad(const FName id)
{
	if (Loaded.Contains(id))
	{
		return Loaded[id];
	}

	if (!Paths.Contains(id))
	{
		DevLogger::GetLoggingChannel()->LogValue("Unknown path. Asset id:", id);
		return nullptr;
	}

	auto const path = Paths[id];
	auto const object = StaticLoadObject(UObject::StaticClass(), nullptr, *path);
	if (!object)
	{
		DevLogger::GetLoggingChannel()->LogValue("Can't load asset. Asset path:", path, LogSeverity::Error);
		return nullptr;
	}
	auto const tObject = Cast<T>(object);

	if (!tObject)
	{
		auto const className = T::StaticClass()->GetName();
		DevLogger::GetLoggingChannel()->LogValue("Can't cast loaded asset to desired type. Class name:", className, LogSeverity::Error);
		return nullptr;
	}

	Loaded.Add(id, tObject);
	
	return tObject;
}

template <class T>
void CachedAssetFactory<T>::LoadAllAssets()
{
	for (auto const asset : Paths)
	{
		GetOrLoad(asset.Key);
	}
}
