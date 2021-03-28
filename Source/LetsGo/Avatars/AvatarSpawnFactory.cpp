#include "AvatarSpawnFactory.h"

#include "LetsGo/Utils/AssetUtils.h"


AvatarSpawnFactory::AvatarSpawnFactory(AvatarFactory* avatarFactory, SkinFactory* skinFactory) :
	_avatarFactory(avatarFactory),
	_skinFactory(skinFactory)
{
}

AAvatar* AvatarSpawnFactory::SpawnAvatar(const AvatarData* avatarData, UWorld* world, const FTransform transform) const
{
	auto const avatarType = avatarData->GetAvatarType();
	auto const avatarGeneratedClass = GetAvatarGeneratedClass(avatarType);

	if (!avatarGeneratedClass)
	{
		return nullptr;
	}
	auto const avatar = AssetUtils::SpawnBlueprint<AAvatar>(
		world,
		nullptr,
		avatarGeneratedClass,
		transform,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);
	if(!avatar)
	{
		return nullptr;
	}

	avatar->Init(avatarData->GetPlayerId(), avatarData->GetAvatarType());
	
	auto const skinId = avatarData->GetSkinId();
	_skinFactory->SetSkin(avatar, skinId);
	
	return avatar;
}

UBlueprintGeneratedClass* AvatarSpawnFactory::GetAvatarGeneratedClass(const AvatarType avatarType) const
{
	switch (avatarType)
	{
		case AvatarType::LocalPlayer:
			return  _avatarFactory->GetLocalPlayerBlueprint();

		case AvatarType::Bot:
			return _avatarFactory->GetBotBlueprint();
		default:
			DevLogger::GetLoggingChannel()->LogValue("Unhandled AvatarType:", static_cast<int>(avatarType), LogSeverity::Error);
			return nullptr;
	}
}
