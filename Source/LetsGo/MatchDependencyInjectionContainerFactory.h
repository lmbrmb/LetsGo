#pragma once

#include "Misc/TypeContainer.h"
#include "LetsGo/Items/HealthItemFactory.h"
#include "LetsGo/Items/GunItemFactory.h"
#include "LetsGo/Items/AmmoItemFactory.h"
#include "LetsGo/PickupItems/PickupItemFactory.h"
#include "LetsGo/WeaponSystem/GunFactory.h"
#include "LetsGo/Avatars/AvatarFactory.h"

class MatchDependencyInjectionContainerFactory final
{
public:
	MatchDependencyInjectionContainerFactory() = default;
	
	~MatchDependencyInjectionContainerFactory() = default;

	template<ESPMode Mode = ESPMode::Fast>
	TTypeContainer<Mode>* CreateContainer();
};

template <ESPMode Mode>
TTypeContainer<Mode>* MatchDependencyInjectionContainerFactory::CreateContainer()
{
	auto const container = new TTypeContainer<Mode>();

	const TSharedRef<PickupItemFactory> pickupItemFactory = MakeShareable(new PickupItemFactory());
	const TSharedRef<GunItemFactory> gunItemFactory = MakeShareable(new GunItemFactory());
	const TSharedRef<HealthItemFactory> healthItemFactory = MakeShareable(new HealthItemFactory());
	const TSharedRef<AmmoItemFactory> ammoItemFactory = MakeShareable(new AmmoItemFactory());
	const TSharedRef<GunFactory> gunFactory = MakeShareable(new GunFactory());
	const TSharedRef<AvatarFactory> avatarFactory = MakeShareable(new AvatarFactory());
	
	container->template RegisterInstance<PickupItemFactory>(pickupItemFactory);
	container->template RegisterInstance<GunItemFactory>(gunItemFactory);
	container->template RegisterInstance<AmmoItemFactory>(ammoItemFactory);
	container->template RegisterInstance<HealthItemFactory>(healthItemFactory);
	container->template RegisterInstance<GunFactory>(gunFactory);
	container->template RegisterInstance<AvatarFactory>(avatarFactory);
	
	return container;
}
