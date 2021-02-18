#include "InventoryToWeaponManagerMapping.h"
#include "LetsGo/InventorySystem/InventoryComponent.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"
#include "LetsGo/Logs/DevLogger.h"

void UInventoryToWeaponManagerMapping::Map()
{
	auto const owner = GetOwner();
	auto const inventoryComponent = owner->FindComponentByClass<UInventoryComponent>();

	if(!inventoryComponent)
	{
		DevLogger::GetLoggingChannel()->LogValue(
			"Component is not found: ", UInventoryComponent::StaticClass()->GetName(),
			LogSeverity::Error
		);
		return;
	}

	auto const weaponManagerComponent = owner->FindComponentByClass<UWeaponManagerComponent>();
	
	if (!weaponManagerComponent)
	{
		DevLogger::GetLoggingChannel()->LogValue(
			"Component is not found: ", UWeaponManagerComponent::StaticClass()->GetName(),
			LogSeverity::Error
		);
		return;
	}
	
	inventoryComponent->ItemAdded.AddUObject(weaponManagerComponent, &UWeaponManagerComponent::OnInventoryItemAdded);
}
