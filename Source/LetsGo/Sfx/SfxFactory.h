#pragma once

#include "LetsGo/AssetFactories/CachedAssetFactory.h"

class SfxFactory final : public CachedAssetFactory<USoundBase>
{
public:
	SfxFactory();
};

Expose_TNameOf(SfxFactory)
