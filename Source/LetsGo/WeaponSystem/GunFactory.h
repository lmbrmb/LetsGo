#pragma once

#include "LetsGo/BlueprintFactory.h"

class LETSGO_API GunFactory final : public BlueprintFactory
{
public:
	explicit GunFactory();
};

Expose_TNameOf(GunFactory)
