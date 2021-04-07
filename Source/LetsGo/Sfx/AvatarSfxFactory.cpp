#include "AvatarSfxFactory.h"

const FName KachujinSkinId = FName("Kachujin");

const FName VampireSkinId = FName("Kachujin");

const FName MariaSkinId = FName("Kachujin");

AvatarSfxFactory::AvatarSfxFactory(SfxFactory* sfxFactory)
{
	//TODO:
}

USoundBase* AvatarSfxFactory::GetJumpSfx()
{
	return nullptr;
}
