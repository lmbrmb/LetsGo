#pragma once

#include "CachedAssetFactory.h"

/// <summary>
/// Gun factory
/// </summary>
class LETSGO_API GunFactory final : public CachedAssetFactory<UBlueprintGeneratedClass>
{
public:
	explicit GunFactory(const bool lazyInitialization);
};

Expose_TNameOf(GunFactory)
