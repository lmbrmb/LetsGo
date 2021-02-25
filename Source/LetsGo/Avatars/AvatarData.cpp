#include "AvatarData.h"

AvatarData::AvatarData(FGuid guid, AvatarType avatarType)
{
	_playerId = guid;
	_avatarType = avatarType;
}

FGuid AvatarData::GetPlayerId() const
{
	return _playerId;
}

AvatarType AvatarData::GetAvatarType() const
{
	return _avatarType;
}
