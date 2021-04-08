#include "PickupManagerComponent.h"

#include "LetsGo/GameModes/ProjectGameModeBase.h"
#include "LetsGo/Items/Item.h"
#include "LetsGo/Items/AmmoItemFactory.h"
#include "LetsGo/Items/HealthItemFactory.h"
#include "LetsGo/Items/GunItemFactory.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/FactoryUtils.h"

UPickupManagerComponent::UPickupManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const projectGameModeBase = Cast<AProjectGameModeBase>(authGameMode);
	AssertIsNotNull(projectGameModeBase);
	auto const diContainer = projectGameModeBase->GetDiContainer();
	
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
		DevLogger::GetLoggingChannel()->LogValue("Item is not created. Item ID:", itemId, LogSeverity::Error);
		return false;
	}

	auto isProcessed = false;

	for (auto itemProcessor : _itemProcessors)
	{
		isProcessed = itemProcessor->TryProcessItem(item);
		if(isProcessed)
		{
			break;
		}
	}

	if(isProcessed)
	{
		ItemPickedUp.Broadcast(item);
	}

	return isProcessed;
}

void UPickupManagerComponent::RegisterItemProcessor(IItemProcessor* itemProcessor)
{
	_itemProcessors.Add(itemProcessor);
}
