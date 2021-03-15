#include "PickupManagerToHealthManagerMapping.h"

#include "LetsGo/Pickups/PickupManagerComponent.h"
#include "LetsGo/HealthSystem/HealthManagerComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

void UPickupManagerToHealthManagerMapping::Map()
{
	auto const owner = GetOwner();
	
	auto const pickupManagerComponent = owner->FindComponentByClass<UPickupManagerComponent>();
	AssertIsNotNull(pickupManagerComponent);
	
	auto const healthManagerComponent = owner->FindComponentByClass<UHealthManagerComponent>();
	AssertIsNotNull(healthManagerComponent);

	pickupManagerComponent->RegisterItemProcessor(healthManagerComponent);
	StartDestroyTask();
}
