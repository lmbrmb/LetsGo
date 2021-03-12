#include "AvatarDataFactory.h"

AvatarDataFactory::AvatarDataFactory(INicknameGenerator* nicknameGenerator) :
	_nicknameGenerator(nicknameGenerator)
{
	_avatarSkinIds.Add("Kachujin");
	_avatarSkinIds.Add("Maria");
	_avatarSkinIds.Add("Vampire");

	// Random start index id
	_skinIndex = FMath::RandRange(0, _avatarSkinIds.Num() - 1);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
AvatarData* AvatarDataFactory::Create(
	const int32 playerId, 
	const AvatarType avatarType, 
	const FName skinId,
	const FName nickname
)
{
	return new AvatarData(playerId, avatarType, skinId, nickname);
}

AvatarData* AvatarDataFactory::GenerateRandom(const int32 playerId, const AvatarType avatarType)
{
	auto const skinId = GetRandomSkinId();
	auto const nickname = _nicknameGenerator->Generate();
	return new AvatarData(playerId, avatarType, skinId, nickname);
}

FName AvatarDataFactory::GetRandomSkinId()
{
	//Cycle skin index
	auto const skinCount = _avatarSkinIds.Num();
	auto nextIndex = _skinIndex + 1;

	if (nextIndex >= skinCount)
	{
		nextIndex = 0;
	}

	_skinIndex = nextIndex;
	return _avatarSkinIds[_skinIndex];
}
