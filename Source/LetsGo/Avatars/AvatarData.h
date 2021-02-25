#pragma once

#include "AvatarType.h"

class AvatarData
{
public:
	explicit AvatarData(FGuid playerId, AvatarType avatarType);

	FGuid GetPlayerId() const;

	AvatarType GetAvatarType() const;
private:
	FGuid _playerId;

	AvatarType _avatarType;
};
