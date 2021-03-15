#include "PickupManagerToWeaponManagerMapping.h"

#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Pickups/PickupManagerComponent.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"
#include "LetsGo/Logs/DevLogger.h"

void UPickupManagerToWeaponManagerMapping::Map()
{
	auto const owner = GetOwner();
	
	auto const pickupManagerComponent = owner->FindComponentByClass<UPickupManagerComponent>();
	AssertIsNotNull(pickupManagerComponent);

	auto const weaponManagerComponent = owner->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManagerComponent);
	
	pickupManagerComponent->RegisterItemProcessor(weaponManagerComponent);
	StartDestroyTask();
}
