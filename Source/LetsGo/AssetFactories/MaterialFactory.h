#pragma once

#include "CachedAssetFactory.h"

/// <summary>
/// Material factory
/// </summary>
class MaterialFactory : public CachedAssetFactory<UMaterial>
{
public:
	explicit MaterialFactory(const bool lazyInitialization);

private:
	static const FString MIXAMO_PATH_PREFIX;
};

Expose_TNameOf(MaterialFactory)