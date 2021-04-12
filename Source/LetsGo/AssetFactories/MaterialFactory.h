#pragma once

#include "CachedAssetFactory.h"

/// <summary>
/// Material factory
/// </summary>
class MaterialFactory : public CachedAssetFactory<UMaterialInterface>
{
public:
	explicit MaterialFactory(
		IUObjectRegistry* uObjectRegistry,
		const bool lazyInitialization
	);

private:
	static const FString MIXAMO_PATH_PREFIX;
};

Expose_TNameOf(MaterialFactory)