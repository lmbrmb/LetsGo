#pragma once

#include "GameFramework/Pawn.h"

#include "AvatarType.h"
#include "LetsGo/Data/PlayerId.h"

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
	
	void Init(const PlayerId& playerId, const AvatarType avatarType);

	const PlayerId& GetPlayerId() const;

	AvatarType GetAvatarType() const;

	EAvatarInitialized Initialized;
	
private:
	PlayerId _playerId;

	AvatarType _avatarType;
};
