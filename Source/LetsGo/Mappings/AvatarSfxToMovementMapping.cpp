#include "AvatarSfxToMovementMapping.h"

#include "LetsGo/Movement/MovementComponentBase.h"
#include "LetsGo/Sfx/AvatarSfxComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAvatarSfxToMovementMapping::Map()
{
	auto const owner = GetOwner();

	auto const avatarSfxComponent = owner->FindComponentByClass<UAvatarSfxComponent>();
	AssertIsNotNull(avatarSfxComponent);

	auto const movementComponentBase = owner->FindComponentByClass<UMovementComponentBase>();
	AssertIsNotNull(movementComponentBase);

	movementComponentBase->Step.AddUObject(avatarSfxComponent, &UAvatarSfxComponent::OnStep);
	movementComponentBase->Jump.AddUObject(avatarSfxComponent, &UAvatarSfxComponent::OnJump);
	movementComponentBase->Land.AddUObject(avatarSfxComponent, &UAvatarSfxComponent::OnLand);

	StartDestroyTask();
}
