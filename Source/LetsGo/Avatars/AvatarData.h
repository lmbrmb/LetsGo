#pragma once

#include "AvatarType.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/SkinId.h"
#include "LetsGo/Data/TeamId.h"

class AvatarData
{
public:
	explicit AvatarData(
		const PlayerId& playerId,
		const AvatarType avatarType,
		const SkinId& skinId, 
		const FName& nickname,
		const TeamId& teamId
	);

	const PlayerId& GetPlayerId() const;

	AvatarType GetAvatarType() const;
	
	const SkinId& GetSkinId() const;

	const FName& GetNickname() const;

	const TeamId& GetTeamId() const;

private:
	PlayerId _playerId;

	AvatarType _avatarType;

	SkinId _skinId;

	FName _nickname;

	TeamId _teamId;
};
