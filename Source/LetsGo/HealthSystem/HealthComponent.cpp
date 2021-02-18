#include "HealthComponent.h"
#include "LetsGo/InventorySystem/HealthItem.h"

void UHealthComponent::OnInventoryItemAdded(InventoryItem* item)
{
	auto const healthItem = dynamic_cast<HealthItem*>(item);
	if (healthItem == nullptr)
	{
		return;
	}

	auto const healAmount = healthItem->GetHealAmount();
	Heal(healAmount);
}

void UHealthComponent::Heal(float healAmount)
{
	if(IsDead())
	{
		return;
	}

	ChangeValue(healAmount);
}

void UHealthComponent::Injure(float damageAmount)
{
	if (IsDead())
	{
		return;
	}
	
	ChangeValue(damageAmount);
}

void UHealthComponent::Kill()
{
	MinimizeCurrentValue();
}

bool UHealthComponent::IsAlive() const
{
	return CurrentValue > 0;
}

bool UHealthComponent::IsDead() const
{
	return !IsAlive();
}
