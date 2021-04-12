#pragma once

#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Data/IUObjectRegistry.h"

/// <summary>
/// [Abstract] Loads assets, returns asset by id 
/// </summary>
/// <typeparam name="T">Asset type</typeparam>
template <class T>
class CachedAssetFactory
{
public:
	T* GetOrLoad(const FName& id);

protected:
	explicit CachedAssetFactory(IUObjectRegistry* uObjectRegistry);

	TMap<const FName, FString> Paths;

	void LoadAllAssets();

private:
	IUObjectRegistry* _uObjectRegistry;

	TMap<const FName, T*> Loaded;
};

template <class T>
T* CachedAssetFactory<T>::GetOrLoad(const FName& id)
{
	if (Loaded.Contains(id))
	{
		return Loaded[id];
	}

	if (!Paths.Contains(id))
	{
		DevLogger::GetLoggingChannel()->LogValue("Unknown path. Asset id:", id, LogSeverity::Error);
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
		DevLogger::GetLoggingChannel()->Log("Can't cast loaded asset to desired type.", LogSeverity::Error);
		DevLogger::GetLoggingChannel()->LogValue("Loaded object type:", object->GetClass()->GetName(), LogSeverity::Error);
		DevLogger::GetLoggingChannel()->LogValue("Desired cast type:", T::StaticClass()->GetName(), LogSeverity::Error);
		
		return nullptr;
	}

	_uObjectRegistry->RegisterUObject(tObject);
	Loaded.Add(id, tObject);
	return tObject;
}

template <class T>
CachedAssetFactory<T>::CachedAssetFactory(IUObjectRegistry* uObjectRegistry)
{
	_uObjectRegistry = uObjectRegistry;
}

template <class T>
void CachedAssetFactory<T>::LoadAllAssets()
{
	for (auto const asset : Paths)
	{
		GetOrLoad(asset.Key);
	}
}
