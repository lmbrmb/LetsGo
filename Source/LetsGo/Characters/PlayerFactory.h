#pragma once

#include "LetsGo/BlueprintFactory.h"

///<summary>
/// Bot factory
///</summary>
class PlayerFactory final : public BlueprintFactory
{
public:
	PlayerFactory();
};

Expose_TNameOf(PlayerFactory)