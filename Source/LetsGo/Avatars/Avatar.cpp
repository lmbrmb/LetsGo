#include "Avatar.h"

AAvatar::AAvatar()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AAvatar::IsInitialized() const
{
	return _playerId.IsValid();
}

void AAvatar::Init(const FGuid playerId, const AvatarType avatarType)
{
	_playerId = playerId;
	_avatarType = avatarType;
	Initialized.Broadcast(this);
}

FGuid AAvatar::GetPlayerId() const
{
	return _playerId;
}

AvatarType AAvatar::GetAvatarType() const
{
	return _avatarType;
}
