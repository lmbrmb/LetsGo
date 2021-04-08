#include "AvatarSfxToHealthMapping.h"

#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Sfx/AvatarSfxComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAvatarSfxToHealthMapping::Map()
{
	auto const actor = GetOwner();

	auto const avatarSfxComponent = actor->FindComponentByClass<UAvatarSfxComponent>();
	AssertIsNotNull(avatarSfxComponent);

	auto const healthComponent = actor->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);

	healthComponent->HealthChanged.AddUObject(avatarSfxComponent, &UAvatarSfxComponent::OnHealthChanged);

	StartDestroyTask();
}
