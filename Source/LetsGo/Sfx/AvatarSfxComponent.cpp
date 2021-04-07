#include "AvatarSfxComponent.h"


#include "AvatarSfxFactory.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

UAvatarSfxComponent::UAvatarSfxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAvatarSfxComponent::SetSkinId(const SkinId& skinId)
{
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(matchGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();

	auto const avatarSfxFactoryRef = diContainer->GetInstance<AvatarSfxFactory>();
	auto const avatarSfxFactory = &avatarSfxFactoryRef.Get();

	auto const jumpSound = avatarSfxFactory->GetJumpSound(skinId);
	auto const landSound = avatarSfxFactory->GetLandSound(skinId);
	auto const stepSounds = avatarSfxFactory->GetStepSounds(skinId);

	BpSetJumpSound(jumpSound);
	BpSetLandSound(landSound);

	for (auto stepSound : stepSounds)
	{
		BpAddStepSound(stepSound);
	}
}

void UAvatarSfxComponent::OnStep()
{
	BpOnStep();
}

void UAvatarSfxComponent::OnJump()
{
	BpOnJump();
}

void UAvatarSfxComponent::OnLand()
{
	BpOnLand();
}
