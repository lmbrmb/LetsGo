#include "Avatar.h"

AAvatar::AAvatar()
{
	PrimaryActorTick.bCanEverTick = false;
}

AvatarData* AAvatar::GetAvatarData() const
{
	return _avatarData;
}

void AAvatar::SetAvatarData(AvatarData* avatarData)
{
	_avatarData = avatarData;
}
