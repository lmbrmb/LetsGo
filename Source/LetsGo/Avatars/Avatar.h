#pragma once

#include "GameFramework/Pawn.h"

#include "AvatarType.h"

#include "Avatar.generated.h"

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
	
	void Init(const FGuid playerId, const AvatarType avatarType);

	FGuid GetPlayerId() const;

	AvatarType GetAvatarType() const;
	
	DECLARE_EVENT_OneParam(
		IGun,
		EInitialized,
		const AAvatar* avatar
		);

	EInitialized Initialized;
	
private:
	FGuid _playerId;

	AvatarType _avatarType;
};
