#include "PickupManagerToHealthMapping.h"

#include "LetsGo/PickupManagerComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Logs/DevLogger.h"

void UPickupManagerToHealthMapping::Map()
{
	auto const owner = GetOwner();
	
	auto const pickupManagerComponent = owner->FindComponentByClass<UPickupManagerComponent>();
	AssertIsNotNull(pickupManagerComponent);
	
	auto const healthComponent = owner->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);

	pickupManagerComponent->ItemPickedUp.AddUObject(healthComponent, &UHealthComponent::OnItemPickedUp);
}
