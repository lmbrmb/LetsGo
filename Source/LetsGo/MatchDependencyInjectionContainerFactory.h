#pragma once

#include "Misc/TypeContainer.h"
#include "LetsGo/InventorySystem/HealthItemFactory.h"
#include "LetsGo/InventorySystem/WeaponItemFactory.h"
#include "LetsGo/PickupItems/PickupItemFactory.h"
#include "LetsGo/WeaponSystem/WeaponFactory.h"
#include "LetsGo/Avatars/AvatarFactory.h"

class MatchDependencyInjectionContainerFactory final
{
public:
	MatchDependencyInjectionContainerFactory();
	
	~MatchDependencyInjectionContainerFactory();

	template<ESPMode Mode = ESPMode::Fast>
	TTypeContainer<Mode>* CreateContainer();
};

template <ESPMode Mode>
TTypeContainer<Mode>* MatchDependencyInjectionContainerFactory::CreateContainer()
{
	auto const container = new TTypeContainer<Mode>();

	const TSharedRef<PickupItemFactory> pickupItemFactory = MakeShareable(new PickupItemFactory());
	const TSharedRef<WeaponItemFactory> weaponItemFactory = MakeShareable(new WeaponItemFactory());
	const TSharedRef<HealthItemFactory> healthItemFactory = MakeShareable(new HealthItemFactory());
	const TSharedRef<WeaponFactory> weaponFactory = MakeShareable(new WeaponFactory());
	const TSharedRef<AvatarFactory> avatarFactory = MakeShareable(new AvatarFactory());
	
	container->template RegisterInstance<PickupItemFactory>(pickupItemFactory);
	container->template RegisterInstance<WeaponItemFactory>(weaponItemFactory);
	container->template RegisterInstance<HealthItemFactory>(healthItemFactory);
	container->template RegisterInstance<WeaponFactory>(weaponFactory);
	container->template RegisterInstance<AvatarFactory>(avatarFactory);
	
	return container;
}
