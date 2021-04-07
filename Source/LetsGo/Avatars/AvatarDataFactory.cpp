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
	const PlayerId& playerId, 
	const AvatarType avatarType, 
	const SkinId& skinId,
	const FName& nickname,
	const TeamId& teamId
)
{
	return new AvatarData(playerId, avatarType, skinId, nickname, teamId);
}

AvatarData* AvatarDataFactory::GenerateRandom(
	const PlayerId& playerId,
	const AvatarType avatarType,
	const TeamId& teamId
)
{
	auto const skinId = GetRandomSkinId();
	auto const nickname = _nicknameGenerator->Generate();
	return new AvatarData(playerId, avatarType, skinId, nickname, teamId);
}

SkinId AvatarDataFactory::GetRandomSkinId()
{
	//Cycle skin index
	auto const skinCount = _avatarSkinIds.Num();
	auto nextIndex = _skinIndex + 1;

	if (nextIndex >= skinCount)
	{
		nextIndex = 0;
	}

	_skinIndex = nextIndex;
	auto const skinIdValue = _avatarSkinIds[_skinIndex];
	return SkinId(skinIdValue);
}
