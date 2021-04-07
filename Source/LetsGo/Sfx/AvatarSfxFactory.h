#pragma once

#include "SfxFactory.h"

class AvatarSfxFactory final
{
public:
	AvatarSfxFactory(SfxFactory* sfxFactory);

private:
	static const FString KachujinSkinId;

	static const FString VampireSkinId;

	static const FString MariaSkinId;

	USoundBase* GetJumpSfx();

	// Skin Id, Sound
	TMap<FName, USoundBase*> _jumpSounds;
};

Expose_TNameOf(AvatarSfxFactory)