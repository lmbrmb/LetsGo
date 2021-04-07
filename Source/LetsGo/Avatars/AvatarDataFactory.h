#pragma once
#include "AvatarData.h"
#include "LetsGo/NicknameGenerators/INicknameGenerator.h"

/// <summary>
/// Avatar data factory
/// </summary>
class AvatarDataFactory final
{
public:
	// ReSharper disable once CppNonExplicitConvertingConstructor
	AvatarDataFactory(INicknameGenerator* nicknameGenerator);

	AvatarData* Create(
		const PlayerId& playerId,
		const AvatarType avatarType,
		const SkinId& skinId,
		const FName& nickname,
		const TeamId& teamId
	);
	
	AvatarData* GenerateRandom(
		const PlayerId& playerId,
		const AvatarType avatarType,
		const TeamId& teamId
	);

private:
	TArray<FName> _avatarSkinIds;
	
	SkinId GetRandomSkinId();

	int _skinIndex;

	INicknameGenerator* _nicknameGenerator;
};

Expose_TNameOf(AvatarDataFactory)
