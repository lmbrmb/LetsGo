#include "AvatarSfxToHealthMapping.h"

#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Sfx/AvatarSfxComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAvatarSfxToHealthMapping::Map()
{
	auto const owner = GetOwner();

	auto const avatarSfxComponent = owner->FindComponentByClass<UAvatarSfxComponent>();
	AssertIsNotNull(avatarSfxComponent);

	auto const healthComponent = owner->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);

	healthComponent->HealthChanged.AddUObject(avatarSfxComponent, &UAvatarSfxComponent::OnHealthChanged);

	StartDestroyTask();
}
