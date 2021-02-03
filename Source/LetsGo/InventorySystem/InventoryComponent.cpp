#include "InventoryComponent.h"
#include "WeaponInventoryItemFactory.h"
#include "LetsGo/Logs/DevLogger.h"
#include "Kismet/KismetStringLibrary.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// TODO: inject factory
	_inventoryItemFactories.Add(new WeaponInventoryItemFactory());
}

InventoryItem* UInventoryComponent::GetInventoryItem() const
{
	return nullptr;
}

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::TryAddItem(FName itemId)
{
	auto const inventoryItem = CreateOnlyOneItem(itemId);
	if(inventoryItem == nullptr)
	{
		return false;
	}

	//TODO: check item existance, max item count, conversion
	
	_inventoryItems.Add(inventoryItem);
	ItemAdded.Broadcast(itemId);
	ItemAddedDelegate.Broadcast(itemId);
	
	return true;
}

bool UInventoryComponent::TryRemoveItem(FName itemId)
{
	auto const inventoryItemPtrToPtr = _inventoryItems.FindByPredicate(
		[itemId](const InventoryItem* ii) {return ii->GetId() == itemId; }
	);

	if(inventoryItemPtrToPtr == nullptr)
	{
		DevLogger::GetLoggingChannel()->LogValue("Can't find inventory item by id: ", itemId, LogSeverity::Error);
		return false;
	}
	
	auto const inventoryItemPtr = *inventoryItemPtrToPtr;
	
	ItemRemoved.Broadcast(itemId);
	ItemRemovedDelegate.Broadcast(itemId);
	
	_inventoryItems.Remove(inventoryItemPtr);
	
	return true;
}

// TODO: to utils
InventoryItem* UInventoryComponent::CreateOnlyOneItem(FName itemId)
{
	TArray<InventoryItem*> createdItems;
	for (auto inventoryItemFactory : _inventoryItemFactories)
	{
		auto const inventoryItem = inventoryItemFactory->Create(itemId);
		if (inventoryItem)
		{
			createdItems.Add(inventoryItem);
		}
	}

	auto const createdItemsCount = createdItems.Num();
	if (createdItemsCount == 0)
	{
		DevLogger::GetLoggingChannel()->Log("No items created for id " + itemId.ToString());
		return nullptr;
	}
	
	if (createdItemsCount > 1)
	{
		DevLogger::GetLoggingChannel()->Log(
			"More than one item created for id " + itemId.ToString() + ". Created "
			+ UKismetStringLibrary::Conv_IntToString(createdItemsCount) + " items.", LogSeverity::Error
		);
		createdItems.Empty();
		return nullptr;
	}
	
	return createdItems[0];
}
