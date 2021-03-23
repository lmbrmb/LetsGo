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
		const FName& nickname,
		const int teamId
	);

	PlayerId GetPlayerId() const;

	AvatarType GetAvatarType() const;
	
	FName GetSkinId() const;

	FName GetNickname() const;

	int GetTeamId() const;

private:
	PlayerId _playerId;

	AvatarType _avatarType;

	FName _skinId;

	const FName _nickname;

	int _teamId;
};
