#pragma once

#include "LetsGo/Logs/DevLogger.h"

class AssetUtils
{
public:
	static UBlueprint* LoadBlueprint(const FString path)
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

		auto const spawnedActor = world->SpawnActor<T>(
			blueprint->GeneratedClass,
			transform,
			spawnParams
			);

		if(!spawnedActor)
		{
			auto const className = T::StaticClass()->GetName();
			DevLogger::GetLoggingChannel()->LogValue("Actor is not spawned. Blueprint class name:", className, LogSeverity::Error);
		}
		
		return spawnedActor;
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

		auto const spawnedActor = world->SpawnActor<T>(
			blueprint->GeneratedClass,
			location,
			rotation,
			spawnParams
			);
		
		if (!spawnedActor)
		{
			auto const className = T::StaticClass()->GetName();
			DevLogger::GetLoggingChannel()->LogValue("Actor is not spawned. Blueprint class name:", className, LogSeverity::Error);
		}

		return spawnedActor;
	}

private:
	AssetUtils() = delete;
};
