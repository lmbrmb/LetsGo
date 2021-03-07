#pragma once

#include "LetsGo/BlueprintFactory.h"

///<summary>
///Pickup item factory
///</summary>
class LETSGO_API PickupItemFactory final : public BlueprintFactory
{
public:
	explicit PickupItemFactory(const bool lazyInitialization);
};

Expose_TNameOf(PickupItemFactory)
