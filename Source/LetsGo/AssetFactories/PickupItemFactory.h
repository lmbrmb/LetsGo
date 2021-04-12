#pragma once

#include "CachedAssetFactory.h"

///<summary>
///Pickup item factory
///</summary>
class LETSGO_API PickupItemFactory final : public CachedAssetFactory<UBlueprintGeneratedClass>
{
public:
	explicit PickupItemFactory(
		IUObjectRegistry* uObjectRegistry,
		const bool lazyInitialization
	);
	
private:
	static const FString AssetPath;
};

Expose_TNameOf(PickupItemFactory)
