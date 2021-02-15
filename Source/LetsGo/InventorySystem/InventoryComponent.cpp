#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "WeaponInventoryItemFactory.h"
#include "LetsGo/GameModes/LetsGoGameModeBase.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/FactoryUtils.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const gameModeBase = dynamic_cast<ALetsGoGameModeBase*, AGameModeBase>(authGameMode);
	auto const diContainer = gameModeBase->GetDiContainer();
	auto const weaponInventoryItemFactory = diContainer->GetInstance<WeaponInventoryItemFactory>();
	_inventoryItemFactories.Add(&weaponInventoryItemFactory.Get());
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

	_inventoryItems.Add(inventoryItem);
	ItemAdded.Broadcast(inventoryItem);
	
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
