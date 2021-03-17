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
		const FName& skinId,
		const FName& nickname
	);
	
	AvatarData* GenerateRandom(const PlayerId& playerId, AvatarType avatarType);

private:
	TArray<FName> _avatarSkinIds;
	
	FName GetRandomSkinId();

	int _skinIndex;

	INicknameGenerator* _nicknameGenerator;
};

Expose_TNameOf(AvatarDataFactory)
