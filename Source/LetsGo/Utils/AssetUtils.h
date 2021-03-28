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
		FTransform transform,
		ESpawnActorCollisionHandlingMethod spawnActorCollisionHandlingMethod
	)
	{
		auto const spawnParams = GetSpawnParameters(owner, spawnActorCollisionHandlingMethod);

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
		ESpawnActorCollisionHandlingMethod spawnActorCollisionHandlingMethod,
		FVector location = FVector::ZeroVector,
		FRotator rotation = FRotator::ZeroRotator,
		FVector scale = FVector::OneVector
	)
	{
		return SpawnBlueprint<T>(
			world,
			owner,
			theClass,
			FTransform(rotation, location, scale),
			spawnActorCollisionHandlingMethod
			);
	}

	static FString GenerateAssetPath(const FString& path, const FString& name)
	{
		return path + name + "." + name + "_C";
	}

private:
	AssetUtils() = delete;

	static FActorSpawnParameters GetSpawnParameters(
		AActor* owner,
		ESpawnActorCollisionHandlingMethod spawnActorCollisionHandlingMethod
	)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = owner;
		spawnParams.SpawnCollisionHandlingOverride = spawnActorCollisionHandlingMethod;
		return spawnParams;
	}
};
