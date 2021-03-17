#include "Avatar.h"

#include "LetsGo/Data/PlayerId.h"

AAvatar::AAvatar()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AAvatar::IsInitialized() const
{
	return _playerId.IsValid();
}

void AAvatar::Init(const PlayerId& playerId, const AvatarType avatarType)
{
	_playerId = playerId;
	_avatarType = avatarType;
	Initialized.Broadcast(this);
}

const PlayerId& AAvatar::GetPlayerId() const
{
	return _playerId;
}

AvatarType AAvatar::GetAvatarType() const
{
	return _avatarType;
}
