#pragma once

#include "CachedAssetFactory.h"

/// <summary>
/// Skeletal mesh factory
/// </summary>
class SkeletalMeshFactory : public CachedAssetFactory<USkeletalMesh>
{
public:
	explicit SkeletalMeshFactory(
		IUObjectRegistry* uObjectRegistry,
		const bool lazyInitialization
	);

private:
	static const FString MIXAMO_PATH_PREFIX;
};

Expose_TNameOf(SkeletalMeshFactory)