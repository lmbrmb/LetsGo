#include "AvatarFactory.h"

const FName LOCAL_PLAYER_ID = "LocalPlayer";
const FName BOT_ID = "Bot";

AvatarFactory::AvatarFactory(const bool lazyInitialization)
{
	Paths.Add(LOCAL_PLAYER_ID, "/Game/Assets/Blueprints/Avatars/BP_Avatar_LocalPlayer_FirstPerson");
	Paths.Add(BOT_ID, "/Game/Assets/Blueprints/Avatars/BP_Avatar_Bot");
	
	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}

UBlueprint* AvatarFactory::GetLocalPlayerBlueprint()
{
	return GetOrLoad(LOCAL_PLAYER_ID);
}

UBlueprint* AvatarFactory::GetBotBlueprint()
{
	return GetOrLoad(BOT_ID);
}
