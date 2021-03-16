#pragma once

#include "LetsGo/Logs/DevLogger.h"

class AssetUtils
{
public:
	template <class T>
	static T* SpawnBlueprint(
		UWorld* world,
		AActor* owner,
		UClass* theClass,
		FTransform transform
	)
	{
		auto const spawnParams = GetSpawnParameters(owner);

		auto const spawnedActor = world->SpawnActor<T>(
			theClass,
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
		UClass* theClass,
		FVector location = FVector::ZeroVector,
		FRotator rotation = FRotator::ZeroRotator,
		FVector scale = FVector::OneVector
	)
	{
		return SpawnBlueprint<T>(world, owner, theClass, FTransform(rotation, location, scale));
	}

	static FString GenerateAssetPath(const FString& path, const FString& name)
	{
		return path + name + "." + name + "_C";
	}

private:
	AssetUtils() = delete;

	static FActorSpawnParameters GetSpawnParameters(AActor* owner)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = owner;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		return spawnParams;
	}
};
