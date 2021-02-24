#pragma once

#include "LetsGo/Logs/DevLogger.h"

class AssetUtils
{
public:
	static UBlueprint* LoadBlueprint(FString path)
	{
		auto const assetObject = StaticLoadObject(UObject::StaticClass(), nullptr, *path);
		if (!assetObject)
		{
			DevLogger::GetLoggingChannel()->LogValue("Can't find asset: ", path, LogSeverity::Error);
			return nullptr;
		}

		return Cast<UBlueprint>(assetObject);
	}

	template <class T>
	static T* SpawnBlueprint(
		UWorld* world,
		AActor* owner,
		UBlueprint* blueprint,
		FTransform transform
	)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = owner;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		return world->SpawnActor<T>(
			blueprint->GeneratedClass,
			transform,
			spawnParams
			);
	}
	
	template <class T>
	static T* SpawnBlueprint(
		UWorld* world,
		AActor* owner,
		UBlueprint* blueprint,
		FVector location = FVector::ZeroVector,
		FRotator rotation = FRotator::ZeroRotator
	)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = owner;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		return world->SpawnActor<T>(
			blueprint->GeneratedClass,
			location,
			rotation,
			spawnParams
			);
	}

private:
	AssetUtils() = delete;
};
