#include "AvatarSfxFactory.h"

#include "LetsGo/Utils/AssertUtils.h"

const FName AvatarSfxFactory::KachujinSkinId = FName("Kachujin");

const FName AvatarSfxFactory::VampireSkinId = FName("Vampire");

const FName AvatarSfxFactory::MariaSkinId = FName("Maria");

const FName AvatarSfxFactory::LandSoundId = FName("Land");

AvatarSfxFactory::AvatarSfxFactory(SfxFactory* sfxFactory)
{
	_sfxFactory = sfxFactory;

	_jumpSounds.Add(KachujinSkinId, "Jump1");
	_jumpSounds.Add(VampireSkinId, "Jump1");
	_jumpSounds.Add(MariaSkinId, "Jump1");

	_stepSounds.Add(KachujinSkinId, "StepNormal");
	_stepSounds.Add(VampireSkinId, "StepFlesh");
	_stepSounds.Add(MariaSkinId, "StepBoot");
}

USoundBase* AvatarSfxFactory::GetJumpSound(SkinId skinId) const
{
	auto const skinIdValue = skinId.GetId();
	AssertIsTrue(_stepSounds.Contains(skinIdValue), nullptr);
	auto const jumpSoundId = _jumpSounds[skinIdValue];
	return _sfxFactory->GetOrLoad(jumpSoundId);
}

USoundBase* AvatarSfxFactory::GetLandSound(SkinId skinId) const
{
	return _sfxFactory->GetOrLoad(LandSoundId);
}

TArray<USoundBase*> AvatarSfxFactory::GetStepSounds(SkinId skinId) const
{
	TArray<USoundBase*> stepSounds;

	auto const skinIdValue = skinId.GetId();
	AssertIsTrue(_stepSounds.Contains(skinIdValue), stepSounds);
	auto const stepSoundIdBase = _stepSounds[skinIdValue];
	
	for(auto i = 1; i <= 4; i++)
	{
		auto const stepSoundId = FName(stepSoundIdBase.ToString() + UKismetStringLibrary::Conv_IntToString(i));
		auto const stepSound = _sfxFactory->GetOrLoad(stepSoundId);
		if(stepSound)
		{
			stepSounds.Add(stepSound);
		}
	}
	
	return stepSounds;
}
