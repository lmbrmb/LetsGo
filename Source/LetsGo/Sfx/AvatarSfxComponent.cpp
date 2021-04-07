#include "AvatarSfxComponent.h"

UAvatarSfxComponent::UAvatarSfxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAvatarSfxComponent::SetSkinId(const SkinId& skinId)
{
	_skinId = skinId;
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
