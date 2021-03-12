#pragma once

#include "AvatarType.h"

class AvatarData
{
public:
	explicit AvatarData(
		const int32 playerId,
		const AvatarType avatarType,
		const FName skinId, 
		const FName nickname
	);

	int32 GetPlayerId() const;

	AvatarType GetAvatarType() const;
	
	FName GetSkinId() const;

	FName GetNickname() const;

private:
	int32 _playerId;

	AvatarType _avatarType;

	FName _skinId;

	const FName _nickname;
};
