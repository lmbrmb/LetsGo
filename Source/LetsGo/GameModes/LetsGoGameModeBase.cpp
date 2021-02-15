#include "LetsGoGameModeBase.h"
#include "LetsGo/InventorySystem/WeaponInventoryItemFactory.h"
#include "LetsGo/PickupItems/PickupItemFactory.h"
#include "LetsGo/WeaponSystem/WeaponFactory.h"

ALetsGoGameModeBase::ALetsGoGameModeBase()
{
	// TODO: DiContainerFactory
	_diContainer = new TTypeContainer<>();
}

ALetsGoGameModeBase::~ALetsGoGameModeBase()
{
	delete _diContainer;
}

void ALetsGoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	ComposeDiContainer();
}

TTypeContainer<ESPMode::Fast>* ALetsGoGameModeBase::GetDiContainer() const
{
	return _diContainer;
}

void ALetsGoGameModeBase::ComposeDiContainer() const
{
	const TSharedRef<PickupItemFactory> pickupItemFactory = MakeShareable(new PickupItemFactory());
	const TSharedRef<WeaponInventoryItemFactory> weaponInventoryItemFactory = MakeShareable(new WeaponInventoryItemFactory());
	const TSharedRef<WeaponFactory> weaponFactory = MakeShareable(new WeaponFactory());
	
	_diContainer->RegisterInstance<PickupItemFactory>(pickupItemFactory);
	_diContainer->RegisterInstance<WeaponInventoryItemFactory>(weaponInventoryItemFactory);
	_diContainer->RegisterInstance<WeaponFactory>(weaponFactory);
}
