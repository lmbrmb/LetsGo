#include "InventoryComponent.h"

#include "AmmoItemFactory.h"
#include "InventoryItem.h"
#include "HealthItemFactory.h"
#include "WeaponItemFactory.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/FactoryUtils.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = dynamic_cast<AMatchGameMode*, AGameModeBase>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();
	
	auto const weaponItemFactory = diContainer->GetInstance<WeaponItemFactory>();
	_inventoryItemFactories.Add(&weaponItemFactory.Get());

	auto const ammoItemFactory = diContainer->GetInstance<AmmoItemFactory>();
	_inventoryItemFactories.Add(&ammoItemFactory.Get());
	
	auto const powerUpInventoryItemFactory = diContainer->GetInstance<HealthItemFactory>();
	_inventoryItemFactories.Add(&powerUpInventoryItemFactory.Get());

	auto const itemConversionFactory = diContainer->GetInstance<ItemConversionFactory>();
	_itemConversionFactory = &itemConversionFactory.Get();
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
	auto createdItem = FactoryUtils::CreateSingle<InventoryItem*, InventoryItemFactory*>(
		_inventoryItemFactories, [itemId](auto* factory) {return factory->Create(itemId); }
	);

	if (createdItem == nullptr)
	{
		DevLogger::GetLoggingChannel()->Log("Item is not created", LogSeverity::Error);
		return false;
	}

	auto hasThisItem = false;
	auto const createdItemInitialQuantity = createdItem->GetQuantity();
	auto const isSingleInstance = createdItem->IsSingleInstance();
	
	// Merge quantity
	for (auto existingItem : _inventoryItems)
	{
		if(existingItem->GetId() != itemId)
		{
			continue;
		}

		hasThisItem = true;
		
		auto const itemQuantity = createdItem->GetQuantity();
		if (itemQuantity <= 0)
		{
			break;
		}

		auto const added = existingItem->IncreaseQuantity(itemQuantity);
		createdItem->DecreaseQuantity(added);

		// Minor search optimization: one found, no need to iterate further because it's single
		if(isSingleInstance)
		{
			break;
		}
	}

	auto const createdItemFinalQuantity = createdItem->GetQuantity();
	auto const isAddedToExisting = createdItemInitialQuantity > createdItemFinalQuantity;
	
	if(hasThisItem && isSingleInstance)
	{
		delete createdItem;
		
		if(isAddedToExisting)
		{
			return true;
		}
		
		// Trying to convert
		auto const convertedItemId = _itemConversionFactory->GetConvertedItemId(itemId);

		if (BoolUtils::IsStringEmpty(convertedItemId))
		{
			// Item can't be converted
			return false;
		}

		// Conversion succeeded - adding converted item
		return TryAddItem(convertedItemId);
	}

	// Merged
	if(createdItemFinalQuantity <= 0)
	{
		DevLogger::GetLoggingChannel()->Log("ItemWasted. " + createdItem->GetId().ToString(), LogSeverity::Error);
		delete createdItem;
		return true;
	}
	
	auto const isConsumable = createdItem->IsConsumable();

	if (!isConsumable)
	{
		_inventoryItems.Add(createdItem);
	}

	ItemAdded.Broadcast(createdItem);

	if (isConsumable)
	{
		delete createdItem;
		return true;
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
