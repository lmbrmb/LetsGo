#include "InventoryToHealthMapping.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/InventorySystem/InventoryComponent.h"
#include "LetsGo/Logs/DevLogger.h"

UInventoryToHealthMapping::UInventoryToHealthMapping()
{
	//Do nothing
}

void UInventoryToHealthMapping::Map()
{
	auto const owner = GetOwner();
	auto const inventoryComponent = owner->FindComponentByClass<UInventoryComponent>();

	if (!inventoryComponent)
	{
		DevLogger::GetLoggingChannel()->LogValue(
			"Component is not found: ", UInventoryComponent::StaticClass()->GetName(),
			LogSeverity::Error
		);
		return;
	}
	
	auto const healthComponent = owner->FindComponentByClass<UHealthComponent>();

	if (!healthComponent)
	{
		DevLogger::GetLoggingChannel()->LogValue(
			"Component is not found: ", UHealthComponent::StaticClass()->GetName(),
			LogSeverity::Error
		);
		return;
	}

	inventoryComponent->ItemAdded.AddUObject(healthComponent, &UHealthComponent::OnInventoryItemAdded);
}
