#pragma once

#include "GameFramework/Pawn.h"

#include "AvatarType.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/SkinId.h"
#include "LetsGo/Data/TeamId.h"

#include "Avatar.generated.h"

class AAvatar;

DECLARE_EVENT_OneParam(AAvatar, EAvatarInitialized, const AAvatar* avatar);

///<summary>
///Avatar. Hero. Player representation
///</summary>
UCLASS()
class LETSGO_API AAvatar : public APawn
{
	GENERATED_BODY()

public:
	AAvatar();

	bool IsInitialized() const;

	void Init(
		const PlayerId& playerId,
		const TeamId& teamId,
		const SkinId& skinId,
		const AvatarType avatarType
	);

	const PlayerId& GetPlayerId() const;

	const TeamId& GetTeamId() const;

	const SkinId& GetSkinId() const;

	AvatarType GetAvatarType() const;

	EAvatarInitialized Initialized;

private:
	PlayerId _playerId;

	TeamId _teamId;

	AvatarType _avatarType;

	SkinId _skinId;
};
