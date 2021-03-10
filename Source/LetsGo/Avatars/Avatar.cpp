#include "Avatar.h"

const int32 AAvatar::UNDEFINED_PLAYER_ID = MIN_int32;

AAvatar::AAvatar()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AAvatar::IsInitialized() const
{
	return _playerId != UNDEFINED_PLAYER_ID;
}

void AAvatar::Init(const int32 playerId, const AvatarType avatarType)
{
	_playerId = playerId;
	_avatarType = avatarType;
	Initialized.Broadcast(this);
}

int32 AAvatar::GetPlayerId() const
{
	return _playerId;
}

AvatarType AAvatar::GetAvatarType() const
{
	return _avatarType;
}
