#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "HealthItemFactory.h"
#include "WeaponItemFactory.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/FactoryUtils.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = dynamic_cast<AMatchGameMode*, AGameModeBase>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();
	
	auto const weaponInventoryItemFactory = diContainer->GetInstance<WeaponItemFactory>();
	_inventoryItemFactories.Add(&weaponInventoryItemFactory.Get());

	auto const powerUpInventoryItemFactory = diContainer->GetInstance<HealthItemFactory>();
	_inventoryItemFactories.Add(&powerUpInventoryItemFactory.Get());
}

InventoryItem* UInventoryComponent::GetInventoryItem(FName itemId) const
{
	auto const inventoryItemPtrToPtr = _inventoryItems.FindByPredicate(
		[itemId](const InventoryItem* ii) {return ii->GetId() == itemId; }
	);
	return inventoryItemPtrToPtr == nullptr ? nullptr : *inventoryItemPtrToPtr;
}

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::TryAddItem(FName itemId)
{
	auto const inventoryItem = FactoryUtils::CreateSingle<InventoryItem*, InventoryItemFactory*>(
		_inventoryItemFactories, [itemId](auto* factory) {return factory->Create(itemId); }
	);
	
	if(inventoryItem == nullptr)
	{
		DevLogger::GetLoggingChannel()->Log("Can't add item", LogSeverity::Error);
		return false;
	}

	//TODO: check item existance, max item count, conversion

	auto const isConsumable = inventoryItem->IsConsumable();
	
	if(!isConsumable)
	{
		_inventoryItems.Add(inventoryItem);
	}
	
	ItemAdded.Broadcast(inventoryItem);

	if(isConsumable)
	{
		delete inventoryItem;
	}
	
	return true;
}

bool UInventoryComponent::TryRemoveItem(FName itemId)
{
	auto const inventoryItem = GetInventoryItem(itemId);

	if(inventoryItem == nullptr)
	{
		DevLogger::GetLoggingChannel()->LogValue("Can't find inventory item by id: ", itemId, LogSeverity::Error);
		return false;
	}
	
	ItemRemoved.Broadcast(inventoryItem);
	_inventoryItems.Remove(inventoryItem);

	return true;
}
