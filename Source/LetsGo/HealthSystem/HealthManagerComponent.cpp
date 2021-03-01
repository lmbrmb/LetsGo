#include "HealthManagerComponent.h"

#include "LetsGo/Items/HealthItem.h"
#include "LetsGo/Utils/AssertUtils.h"

UHealthManagerComponent::UHealthManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UHealthManagerComponent::TryProcessItem(Item* item)
{
	if(!_healthComponent)
	{
		return false;
	}
	
	auto const healthItem = dynamic_cast<HealthItem*>(item);
	if (healthItem == nullptr)
	{
		return false;
	}

	if(_healthComponent->IsDead() || _healthComponent->IsFullHealth())
	{
		return false;
	}
	
	auto const healAmount = healthItem->GetHealAmount();
	_healthComponent->Heal(healAmount);
	
	return true;
}

void UHealthManagerComponent::BeginPlay()
{
	auto const actor = GetOwner();
	_healthComponent = actor->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(_healthComponent);
}
