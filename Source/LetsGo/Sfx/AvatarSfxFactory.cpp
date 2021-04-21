#include "AvatarSfxFactory.h"

#include "LetsGo/Utils/AssertUtils.h"

const FName AvatarSfxFactory::KachujinSkinId = FName("Kachujin");

const FName AvatarSfxFactory::VampireSkinId = FName("Vampire");

const FName AvatarSfxFactory::MariaSkinId = FName("Maria");

const FName AvatarSfxFactory::LandSoundId = FName("Land");

AvatarSfxFactory::AvatarSfxFactory(SfxFactory* sfxFactory)
{
	_sfxFactory = sfxFactory;

	_jumpSounds.Add(VampireSkinId, "UrielJump1");
	_jumpSounds.Add(KachujinSkinId, "HunterJump1");
	_jumpSounds.Add(MariaSkinId, "MinxJump1");

	_stepSounds.Add(VampireSkinId, "StepFlesh");
	_stepSounds.Add(KachujinSkinId, "StepNormal");
	_stepSounds.Add(MariaSkinId, "StepBoot");

	_deathSounds.Add(VampireSkinId, "UrielDeath");
	_deathSounds.Add(KachujinSkinId, "HunterDeath");
	_deathSounds.Add(MariaSkinId, "MinxDeath");

	_painSounds.Add(VampireSkinId, "UrielPain");
	_painSounds.Add(KachujinSkinId, "HunterPain");
	_painSounds.Add(MariaSkinId, "MinxPain");

	_painIds.Add(25);
	_painIds.Add(50);
	_painIds.Add(75);
	_painIds.Add(100);
}

USoundBase* AvatarSfxFactory::GetJumpSound(const SkinId& skinId) const
{
	auto const skinIdValue = skinId.GetId();
	AssertContainerContainsElement(_jumpSounds, skinIdValue, nullptr);
	
	auto const jumpSoundId = _jumpSounds[skinIdValue];
	return _sfxFactory->GetOrLoad(jumpSoundId);
}

USoundBase* AvatarSfxFactory::GetLandSound(const SkinId& skinId) const
{
	return _sfxFactory->GetOrLoad(LandSoundId);
}

TArray<USoundBase*> AvatarSfxFactory::GetStepSounds(const SkinId& skinId) const
{
	return GetSounds(skinId, _stepSounds, 4);
}

TArray<USoundBase*> AvatarSfxFactory::GetDeathSounds(const SkinId& skinId) const
{
	return GetSounds(skinId, _deathSounds, 3);
}

TMap<float, USoundBase*> AvatarSfxFactory::GetPainSounds(const SkinId& skinId) const
{
	TMap<float, USoundBase*> painSounds;
	auto const skinIdValue = skinId.GetId();
	AssertContainerContainsElement(_painSounds, skinIdValue, painSounds);

	auto const soundIdBase = _painSounds[skinIdValue];

	for (auto painId : _painIds)
	{
		auto const soundId = FName(soundIdBase + UKismetStringLibrary::Conv_IntToString(painId));
		auto const sound = _sfxFactory->GetOrLoad(soundId);
		if (sound)
		{
			painSounds.Add(painId, sound);
		}
	}
	
	return painSounds;
}

TArray<USoundBase*> AvatarSfxFactory::GetSounds(
	const SkinId& skinId,
	const TMap<FName, FString>& source,
	const int count
) const
{
	TArray<USoundBase*> sounds;
	auto const skinIdValue = skinId.GetId();
	AssertContainerContainsElement(source, skinIdValue, sounds);

	auto const soundIdBase = source[skinIdValue];

	const int firstSoundIndex = 1;
	for (auto i = firstSoundIndex; i <= count; i++)
	{
		auto const soundId = FName(soundIdBase + UKismetStringLibrary::Conv_IntToString(i));
		auto const sound = _sfxFactory->GetOrLoad(soundId);
		if (sound)
		{
			sounds.Add(sound);
		}
	}

	return sounds;
}
