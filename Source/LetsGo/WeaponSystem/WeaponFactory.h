#pragma once

#include "LetsGo/BlueprintFactory.h"

class LETSGO_API WeaponFactory final : public BlueprintFactory
{
public:
	WeaponFactory();
};

Expose_TNameOf(WeaponFactory)
