#include "AvatarData.h"

#include "LetsGo/Data/PlayerId.h"

AvatarData::AvatarData(
	const PlayerId& playerId,
	const AvatarType avatarType,
	const FName& skinId,
	const FName& nickname,
	const int teamId
) :
	_playerId(playerId),
	_avatarType(avatarType),
	_skinId(skinId),
	_nickname(nickname),
	_teamId(teamId)
{
}

PlayerId AvatarData::GetPlayerId() const
{
	return _playerId;
}

AvatarType AvatarData::GetAvatarType() const
{
	return _avatarType;
}

FName AvatarData::GetSkinId() const
{
	return _skinId;
}

FName AvatarData::GetNickname() const
{
	return _nickname;
}

int AvatarData::GetTeamId() const
{
	return _teamId;
}
