#pragma once

#include "CachedAssetFactory.h"

///<summary>
///Pickup item factory
///</summary>
class LETSGO_API PickupItemFactory final : public CachedAssetFactory<UBlueprintGeneratedClass>
{
public:
	explicit PickupItemFactory(const bool lazyInitialization);
	
private:
	static const FString _assetPath;
};

Expose_TNameOf(PickupItemFactory)
