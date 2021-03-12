#pragma once

#include "LetsGo/BlueprintFactory.h"

///<summary>
/// Avatar blueprint factory
///</summary>
class AvatarFactory final : private BlueprintFactory
{
public:
	explicit AvatarFactory(const bool lazyInitialization);

	UBlueprint* GetLocalPlayerBlueprint();
	
	UBlueprint* GetRemotePlayerBlueprint();
	
	UBlueprint* GetBotBlueprint();
};

Expose_TNameOf(AvatarFactory)