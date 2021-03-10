#include "AvatarData.h"

AvatarData::AvatarData(const int32 playerId, const AvatarType avatarType)
{
	_playerId = playerId;
	_avatarType = avatarType;
}

int32 AvatarData::GetPlayerId() const
{
	return _playerId;
}

AvatarType AvatarData::GetAvatarType() const
{
	return _avatarType;
}
