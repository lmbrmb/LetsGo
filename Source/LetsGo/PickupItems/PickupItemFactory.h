#pragma once

#include "LetsGo/BlueprintFactory.h"

///<summary>
///Pickup item factory
///</summary>
class LETSGO_API PickupItemFactory : public BlueprintFactory
{
public:
	explicit PickupItemFactory();
};

Expose_TNameOf(PickupItemFactory)