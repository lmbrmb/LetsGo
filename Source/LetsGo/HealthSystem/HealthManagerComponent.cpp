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

	auto healAmount = healthItem->GetHealAmount();
	
	auto const canHealAboveNormal = healthItem->GetId() == "MajorHealth";
	if(!canHealAboveNormal)
	{
		auto const normalHealthDelta = _healthComponent->GetMaxNormalHealth() - _healthComponent->GetCurrentValue();
		healAmount = FMath::Min(healAmount, normalHealthDelta);
		
		if(normalHealthDelta < 0)
		{
			return false;
		}
	}
	
	
	_healthComponent->Heal(healAmount);
	
	return true;
}

void UHealthManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	auto const actor = GetOwner();
	_healthComponent = actor->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(_healthComponent);
}
