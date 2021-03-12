#pragma once

#include "CachedAssetFactory.h"

///<summary>
/// Avatar blueprint factory
///</summary>
class AvatarFactory final : private CachedAssetFactory<UBlueprint>
{
public:
	explicit AvatarFactory(const bool lazyInitialization);

	UBlueprint* GetLocalPlayerBlueprint();
	
	UBlueprint* GetRemotePlayerBlueprint();
	
	UBlueprint* GetBotBlueprint();
};

Expose_TNameOf(AvatarFactory)