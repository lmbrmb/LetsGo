#include "AvatarFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

const FName LOCAL_PLAYER_ID = "LocalPlayer";
const FName BOT_ID = "Bot";

const FString AvatarFactory::_assetPath = "/Game/Assets/Blueprints/Avatars/";

AvatarFactory::AvatarFactory(const bool lazyInitialization)
{
	Paths.Add(LOCAL_PLAYER_ID, AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_Avatar_LocalPlayer_FirstPerson"));
	Paths.Add(BOT_ID, AssetUtils::GenerateBlueprintAssetPath(_assetPath, "BP_Avatar_Bot"));
	
	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}

UBlueprintGeneratedClass* AvatarFactory::GetLocalPlayerBlueprint()
{
	return GetOrLoad(LOCAL_PLAYER_ID);
}

UBlueprintGeneratedClass* AvatarFactory::GetBotBlueprint()
{
	return GetOrLoad(BOT_ID);
}
