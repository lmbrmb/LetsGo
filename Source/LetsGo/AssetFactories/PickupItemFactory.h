#pragma once

#include "CachedAssetFactory.h"

///<summary>
///Pickup item factory
///</summary>
class LETSGO_API PickupItemFactory final : public CachedAssetFactory<UBlueprintGeneratedClass>
{
public:
	explicit PickupItemFactory(const bool lazyInitialization);
};

Expose_TNameOf(PickupItemFactory)
