#pragma once

#include "LetsGo/BlueprintFactory.h"

///<summary>
/// Bot factory
///</summary>
class BotFactory final : public BlueprintFactory
{
public:
	BotFactory();
};

Expose_TNameOf(BotFactory)