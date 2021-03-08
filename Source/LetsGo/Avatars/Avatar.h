#pragma once

#include "GameFramework/Pawn.h"
#include "AvatarData.h"

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

	AvatarData* GetAvatarData() const;
	
	void SetAvatarData(AvatarData* avatarData);

	DECLARE_EVENT_OneParam(
		IGun,
		EAvatarDataSet,
		const AvatarData* avatarData
		);

	EAvatarDataSet AvatarDataSet;
	
private:
	AvatarData* _avatarData;
};
