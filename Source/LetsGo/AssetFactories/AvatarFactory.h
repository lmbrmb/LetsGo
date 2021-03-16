#pragma once

#include "CachedAssetFactory.h"

///<summary>
/// Avatar blueprint factory
///</summary>
class AvatarFactory final : private CachedAssetFactory<UBlueprintGeneratedClass>
{
public:
	explicit AvatarFactory(const bool lazyInitialization);

	UBlueprintGeneratedClass* GetLocalPlayerBlueprint();
	
	UBlueprintGeneratedClass* GetBotBlueprint();
};

Expose_TNameOf(AvatarFactory)