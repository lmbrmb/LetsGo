#include "AvatarSfxToMovementMapping.h"

#include "LetsGo/Movement/MovementComponentBase.h"
#include "LetsGo/Sfx/AvatarSfxComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAvatarSfxToMovementMapping::Map()
{
	auto const actor = GetOwner();

	auto const avatarSfxComponent = actor->FindComponentByClass<UAvatarSfxComponent>();
	AssertIsNotNull(avatarSfxComponent);

	auto const movementComponentBase = actor->FindComponentByClass<UMovementComponentBase>();
	AssertIsNotNull(movementComponentBase);

	movementComponentBase->Step.AddUObject(avatarSfxComponent, &UAvatarSfxComponent::OnStep);
	movementComponentBase->Jump.AddUObject(avatarSfxComponent, &UAvatarSfxComponent::OnJump);
	movementComponentBase->Land.AddUObject(avatarSfxComponent, &UAvatarSfxComponent::OnLand);

	StartDestroyTask();
}
