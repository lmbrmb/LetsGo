#pragma once

#include "LetsGo/BlueprintFactory.h"

class LETSGO_API GunFactory final : public BlueprintFactory
{
public:
	explicit GunFactory(const bool lazyInitialization);
};

Expose_TNameOf(GunFactory)
