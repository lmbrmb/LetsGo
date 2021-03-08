#include "HealthComponent.h"

void UHealthComponent::OnChanged()
{
	if(IsDead())
	{
		Died.Broadcast(GetOwner());
	}
}

void UHealthComponent::Heal(const float healAmount)
{
	if(IsDead())
	{
		return;
	}

	auto const healValue = FMath::Abs(healAmount);
	ChangeValue(healValue);
}

void UHealthComponent::Injure(Damage damage)
{
	if (IsDead())
	{
		return;
	}

	auto const damageAmount = damage.GetAmount();
	auto const damageValue = -1 * FMath::Abs(damageAmount);
	//TODO: save last damage
	ChangeValue(damageValue);
}

void UHealthComponent::Kill()
{
	SetCurrentValue(0);
}

bool UHealthComponent::IsAlive() const
{
	return CurrentValue > 0;
}

bool UHealthComponent::IsDead() const
{
	return !IsAlive();
}

bool UHealthComponent::IsFullHealth() const
{
	return FMath::IsNearlyZero(MaxValue - CurrentValue);
}
