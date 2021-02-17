#pragma once
#include "Misc/TypeContainer.h"
#include "LetsGo/InventorySystem/WeaponInventoryItemFactory.h"
#include "LetsGo/PickupItems/PickupItemFactory.h"
#include "LetsGo/WeaponSystem/WeaponFactory.h"

class DependencyInjectionContainerFactory final
{
public:
	DependencyInjectionContainerFactory();
	
	~DependencyInjectionContainerFactory();

	template<ESPMode Mode = ESPMode::Fast>
	TTypeContainer<Mode>* CreateContainer();
};

template <ESPMode Mode>
TTypeContainer<Mode>* DependencyInjectionContainerFactory::CreateContainer()
{
	auto const container = new TTypeContainer<Mode>();

	const TSharedRef<PickupItemFactory> pickupItemFactory = MakeShareable(new PickupItemFactory());
	const TSharedRef<WeaponInventoryItemFactory> weaponInventoryItemFactory = MakeShareable(new WeaponInventoryItemFactory());
	const TSharedRef<WeaponFactory> weaponFactory = MakeShareable(new WeaponFactory());

	container->template RegisterInstance<PickupItemFactory>(pickupItemFactory);
	container->template RegisterInstance<WeaponInventoryItemFactory>(weaponInventoryItemFactory);
	container->template RegisterInstance<WeaponFactory>(weaponFactory);
	
	return container;
}
