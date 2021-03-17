#pragma once

#include "AvatarType.h"
#include "LetsGo/Data/PlayerId.h"

class AvatarData
{
public:
	explicit AvatarData(
		const PlayerId& playerId,
		const AvatarType avatarType,
		const FName& skinId, 
		const FName& nickname
	);

	PlayerId GetPlayerId() const;

	AvatarType GetAvatarType() const;
	
	FName GetSkinId() const;

	FName GetNickname() const;

private:
	PlayerId _playerId;

	AvatarType _avatarType;

	FName _skinId;

	const FName _nickname;
};
