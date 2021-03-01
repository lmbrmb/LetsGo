#pragma once

#include "LetsGo/BlueprintFactory.h"

///<summary>
/// Avatar factory
///</summary>
class AvatarFactory final : private BlueprintFactory
{
public:
	explicit AvatarFactory();
	
	UBlueprint* GetLocalPlayerBlueprint();
	
	UBlueprint* GetRemotePlayerBlueprint();
	
	UBlueprint* GetBotBlueprint();
};

Expose_TNameOf(AvatarFactory)