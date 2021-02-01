#include "InventoryComponent.h"

#include "LetsGo/Logs/DevLogger.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItem(FName itemId)
{
	DevLogger::GetLoggingChannel()->Log("Inventory. AddItem: " + itemId.ToString());
	_itemIds.Add(itemId);
	ItemAdded.Broadcast(itemId);
}

void UInventoryComponent::RemoveItem(FName itemId)
{
	DevLogger::GetLoggingChannel()->Log("Inventory. RemoveItem: " + itemId.ToString());
	_itemIds.Remove(itemId);
	ItemRemoved.Broadcast(itemId);
}
