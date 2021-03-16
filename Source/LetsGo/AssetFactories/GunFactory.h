#pragma once

#include "CachedAssetFactory.h"

/// <summary>
/// Gun factory
/// </summary>
class LETSGO_API GunFactory final : public CachedAssetFactory<UBlueprintGeneratedClass>
{
public:
	explicit GunFactory(const bool lazyInitialization);
	
private:
	static const FString _assetPath;
};

Expose_TNameOf(GunFactory)
