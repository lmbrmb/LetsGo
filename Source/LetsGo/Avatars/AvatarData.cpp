#include "AvatarData.h"

AvatarData::AvatarData(
	const PlayerId& playerId,
	const AvatarType avatarType,
	const SkinId& skinId,
	const FName& nickname,
	const TeamId& teamId
) :
	_playerId(playerId),
	_avatarType(avatarType),
	_skinId(skinId),
	_nickname(nickname),
	_teamId(teamId)
{
}

const PlayerId& AvatarData::GetPlayerId() const
{
	return _playerId;
}

AvatarType AvatarData::GetAvatarType() const
{
	return _avatarType;
}

const SkinId& AvatarData::GetSkinId() const
{
	return _skinId;
}

const FName& AvatarData::GetNickname() const
{
	return _nickname;
}

const TeamId& AvatarData::GetTeamId() const
{
	return _teamId;
}
