#pragma once

#include "CachedAssetFactory.h"

/// <summary>
/// Gun factory
/// </summary>
class LETSGO_API GunFactory final : public CachedAssetFactory<UBlueprintGeneratedClass>
{
public:
	explicit GunFactory(
		IUObjectRegistry* uObjectRegistry,
		const bool lazyInitialization
	);
	
private:
	static const FString AssetPath;
};

Expose_TNameOf(GunFactory)
