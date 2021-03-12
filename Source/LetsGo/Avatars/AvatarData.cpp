#include "AvatarData.h"

AvatarData::AvatarData(
	const int32 playerId,
	const AvatarType avatarType,
	const FName skinId,
	const FName nickName
) :
	_playerId(playerId),
	_avatarType(avatarType),
	_skinId(skinId),
	_nickname(nickName)
{
}

int32 AvatarData::GetPlayerId() const
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
