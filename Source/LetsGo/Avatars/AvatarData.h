#pragma once

#include "AvatarType.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/TeamId.h"

class AvatarData
{
public:
	explicit AvatarData(
		const PlayerId& playerId,
		const AvatarType avatarType,
		const FName& skinId, 
		const FName& nickname,
		const TeamId& teamId
	);

	PlayerId GetPlayerId() const;

	AvatarType GetAvatarType() const;
	
	FName GetSkinId() const;

	FName GetNickname() const;

	TeamId GetTeamId() const;

private:
	PlayerId _playerId;

	AvatarType _avatarType;

	FName _skinId;

	const FName _nickname;

	TeamId _teamId;
};
