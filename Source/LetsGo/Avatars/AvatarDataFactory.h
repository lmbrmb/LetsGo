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
		const int32 playerId,
		const AvatarType avatarType,
		const FName skinId,
		const FName nickname
	);
	
	AvatarData* GenerateRandom(int32 playerId, AvatarType avatarType);

private:
	TArray<FName> _avatarSkinIds;
	
	FName GetRandomSkinId();

	int _skinIndex;

	INicknameGenerator* _nicknameGenerator;
};

Expose_TNameOf(AvatarDataFactory)
