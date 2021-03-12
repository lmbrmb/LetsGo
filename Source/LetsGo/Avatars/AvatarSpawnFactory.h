#pragma once
#include "Avatar.h"
#include "AvatarData.h"
#include "AvatarType.h"
#include "LetsGo/AssetFactories/AvatarFactory.h"
#include "LetsGo/AssetFactories/SkinFactory.h"

/// <summary>
/// Avatar spawn factory. Spawns avatar, initializes, applies skin
/// </summary>
class AvatarSpawnFactory final
{
public:
	// ReSharper disable once CppNonExplicitConvertingConstructor
	AvatarSpawnFactory(AvatarFactory* avatarFactory, SkinFactory* skinFactory);
	
	AAvatar* SpawnAvatar(const AvatarData* avatarData, UWorld* world, const FTransform transform) const;

private:
	AvatarFactory* _avatarFactory;

	SkinFactory* _skinFactory;
	
	UBlueprint* GetAvatarBlueprint(const AvatarType avatarType) const;
};

Expose_TNameOf(AvatarSpawnFactory)