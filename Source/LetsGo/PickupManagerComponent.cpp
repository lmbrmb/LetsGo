#include "PickupManagerComponent.h"

#include "LetsGo/Items/Item.h"
#include "LetsGo/Items/AmmoItemFactory.h"
#include "LetsGo/Items/HealthItemFactory.h"
#include "LetsGo/Items/GunItemFactory.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/FactoryUtils.h"

UPickupManagerComponent::UPickupManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();
	
	auto const gunItemFactory = diContainer->GetInstance<GunItemFactory>();
	_itemFactories.Add(&gunItemFactory.Get());

	auto const ammoItemFactory = diContainer->GetInstance<AmmoItemFactory>();
	_itemFactories.Add(&ammoItemFactory.Get());
	
	auto const healthItemFactory = diContainer->GetInstance<HealthItemFactory>();
	_itemFactories.Add(&healthItemFactory.Get());
}

bool UPickupManagerComponent::TryPickUpItem(FName itemId)
{
	auto const item = FactoryUtils::GetSingle<Item*, IItemFactory*>(
		_itemFactories, [itemId](auto* factory) {return factory->Get(itemId); }
	);

	if (item == nullptr)
	{
		DevLogger::GetLoggingChannel()->Log("Item is not created", LogSeverity::Error);
		return false;
	}

	ItemPickedUp.Broadcast(item);

	return true;
}
