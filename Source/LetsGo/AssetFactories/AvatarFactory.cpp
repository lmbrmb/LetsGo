#include "AvatarFactory.h"

#include "LetsGo/Utils/AssetUtils.h"

FName AvatarFactory::LocalPlayerId = "LocalPlayer";

FName AvatarFactory::BotId = "Bot";

FString AvatarFactory::AssetPath = "/Game/Assets/Blueprints/Avatars/";

AvatarFactory::AvatarFactory(const bool lazyInitialization)
{
	Paths.Add(LocalPlayerId, AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_Avatar_LocalPlayer_FirstPerson"));
	Paths.Add(BotId, AssetUtils::GenerateBlueprintAssetPath(AssetPath, "BP_Avatar_Bot"));

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}

UBlueprintGeneratedClass* AvatarFactory::GetLocalPlayerBlueprint()
{
	return GetOrLoad(LocalPlayerId);
}

UBlueprintGeneratedClass* AvatarFactory::GetBotBlueprint()
{
	return GetOrLoad(BotId);
}
