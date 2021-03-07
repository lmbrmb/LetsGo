#include "AvatarFactory.h"

const FName LOCAL_PLAYER_ID = "LocalPlayer";
const FName REMOTE_PLAYER_ID = "RemotePlayer";
const FName BOT_ID = "Bot";

AvatarFactory::AvatarFactory(const bool lazyInitialization)
	: BlueprintFactory()
{
	Assets.Add(LOCAL_PLAYER_ID, "/Game/Assets/Blueprints/Avatars/BP_Avatar_LocalPlayer_FirstPerson");
	Assets.Add(REMOTE_PLAYER_ID, "/Game/Assets/Blueprints/Avatars/BP_Avatar_RemotePlayer");
	Assets.Add(BOT_ID, "/Game/Assets/Blueprints/Avatars/BP_Avatar_Bot");
	
	if (!lazyInitialization)
	{
		LoadAllBlueprints();
	}
}

UBlueprint* AvatarFactory::GetLocalPlayerBlueprint()
{
	return GetBlueprint(LOCAL_PLAYER_ID);
}

UBlueprint* AvatarFactory::GetRemotePlayerBlueprint()
{
	return GetBlueprint(REMOTE_PLAYER_ID);
}

UBlueprint* AvatarFactory::GetBotBlueprint()
{
	return GetBlueprint(BOT_ID);
}
