#pragma once

#include "SfxFactory.h"
#include "LetsGo/Data/SkinId.h"

class AvatarSfxFactory final
{
public:
	explicit AvatarSfxFactory(SfxFactory* sfxFactory);

	USoundBase* GetJumpSound(SkinId skinId) const;

	USoundBase* GetLandSound(SkinId skinId) const;

	TArray<USoundBase*> GetStepSounds(SkinId skinId) const;

private:
	static const FName KachujinSkinId;

	static const FName VampireSkinId;

	static const FName MariaSkinId;

	static const FName LandSoundId;

	SfxFactory* _sfxFactory;

	// Skin Id, Sound Id
	TMap<FName, FName> _jumpSounds;

	// Skin Id, Base part of sound Id
	TMap<FName, FName> _stepSounds;
};

Expose_TNameOf(AvatarSfxFactory)