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
		const ESpawnActorCollisionHandlingMethod spawnActorCollisionHandlingMethod,
		FTransform transform
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
			auto const className = theClass->GetName();
			auto const castName = T::StaticClass()->GetName();

			DevLogger::GetLoggingChannel()->Log("Actor is not spawned.", LogSeverity::Error);
			DevLogger::GetLoggingChannel()->LogValue("Class:", className, LogSeverity::Error);
			DevLogger::GetLoggingChannel()->LogValue("Cast:", castName, LogSeverity::Error);
		}
		
		return spawnedActor;
	}
	
	template <class T>
	static T* SpawnBlueprint(
		UWorld* world,
		AActor* owner,
		UClass* theClass,
		const ESpawnActorCollisionHandlingMethod spawnActorCollisionHandlingMethod,
		FVector location = FVector::ZeroVector,
		FRotator rotation = FRotator::ZeroRotator,
		FVector scale = FVector::OneVector
	)
	{
		return SpawnBlueprint<T>(
			world,
			owner,
			theClass,
			spawnActorCollisionHandlingMethod,
			FTransform(rotation, location, scale)
			);
	}

	static FString GenerateBlueprintAssetPath(const FString& assetPath, const FString& assetName)
	{
		return GenerateAssetPath(assetPath, assetName) + "_C";
	}

	static FString GenerateAssetPath(const FString& assetPath, const FString& assetName)
	{
		return assetPath + assetName + "." + assetName;
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
