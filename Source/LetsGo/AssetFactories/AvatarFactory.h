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

private:
	static const FString _assetPath;
};

Expose_TNameOf(AvatarFactory)