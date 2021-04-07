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

void AAvatar::Init(
	const PlayerId& playerId,
	const TeamId& teamId,
	const SkinId& skinId,
	const AvatarType avatarType
)
{
	_playerId = playerId;
	_teamId = teamId;
	_skinId = skinId;
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

const SkinId& AAvatar::GetSkinId() const
{
	return _skinId;
}

AvatarType AAvatar::GetAvatarType() const
{
	return _avatarType;
}
