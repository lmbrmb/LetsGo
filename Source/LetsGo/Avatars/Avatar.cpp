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

void AAvatar::Init(const PlayerId& playerId, const TeamId& teamId, const AvatarType avatarType)
{
	_playerId = playerId;
	_teamId = teamId;
	_avatarType = avatarType;
	Initialized.Broadcast(this);
}

const PlayerId& AAvatar::GetPlayerId() const
{
	return _playerId;
}

const TeamId& AAvatar::GetTeamId() const
{
	return _teamId;
}

AvatarType AAvatar::GetAvatarType() const
{
	return _avatarType;
}
