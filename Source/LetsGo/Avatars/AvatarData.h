#pragma once

#include "AvatarType.h"

class AvatarData
{
public:
	explicit AvatarData(const int32 playerId, const AvatarType avatarType);

	int32 GetPlayerId() const;

	AvatarType GetAvatarType() const;
private:
	int32 _playerId;

	AvatarType _avatarType;
};
