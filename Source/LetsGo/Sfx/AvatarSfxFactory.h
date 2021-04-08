#pragma once

#include "SfxFactory.h"
#include "LetsGo/Data/SkinId.h"

class AvatarSfxFactory final
{
public:
	explicit AvatarSfxFactory(SfxFactory* sfxFactory);

	USoundBase* GetJumpSound(const SkinId& skinId) const;

	USoundBase* GetLandSound(const SkinId& skinId) const;

	TArray<USoundBase*> GetStepSounds(const SkinId& skinId) const;

	TArray<USoundBase*> GetDeathSounds(const SkinId& skinId) const;

	// Health, Sound
	TMap<float, USoundBase*> GetPainSounds(const SkinId& skinId) const;

private:
	static const FName KachujinSkinId;

	static const FName VampireSkinId;

	static const FName MariaSkinId;

	static const FName LandSoundId;

	SfxFactory* _sfxFactory;

	// Skin Id, Sound Id
	TMap<FName, FName> _jumpSounds;

	// Skin Id, Base part of sound Id
	TMap<FName, FString> _stepSounds;

	// Skin Id, Base part of sound Id
	TMap<FName, FString> _deathSounds;

	// Skin Id, Base part of sound Id
	TMap<FName, FString> _painSounds;

	TArray<int> _painIds;
	
	TArray<USoundBase*> GetSounds(
		const SkinId& skinId,
		const TMap<FName, FString>& source,
		const int count
	) const;
};

Expose_TNameOf(AvatarSfxFactory)