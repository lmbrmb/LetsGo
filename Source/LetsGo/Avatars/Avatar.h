#pragma once

#include "GameFramework/Pawn.h"

#include "AvatarType.h"

#include "Avatar.generated.h"

class AAvatar;

DECLARE_EVENT_OneParam(IGun, EInitialized, const AAvatar* avatar);

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
	
	void Init(const int32 playerId, const AvatarType avatarType);

	int32 GetPlayerId() const;

	AvatarType GetAvatarType() const;

	EInitialized Initialized;
	
private:
	static const int32 UNDEFINED_PLAYER_ID;
	
	int32 _playerId = UNDEFINED_PLAYER_ID;

	AvatarType _avatarType;
};
