#include "HealthComponent.h"

#include "LetsGo/Utils/AssertUtils.h"

void UHealthComponent::OnChanged(const float delta)
{
	HealthChanged.Broadcast(this, delta);
	BpHealthChanged.Broadcast();
	
	if(IsDead())
	{
		Died.Broadcast(this, delta);
		BpDied.Broadcast();
	}
}

void UHealthComponent::Init()
{
	FTimerHandle decreaseHealthTimerHandle;

	AssertIsGreaterOrEqual(_decreaseHealthInterval, 0.0f)
	AssertIsGreaterOrEqual(_decreaseHealthAmount, 0.0f);
	AssertIsGreaterOrEqual(_decreaseHealthStopValue, 0.0f);
	
	if(_decreaseHealthAmount > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(decreaseHealthTimerHandle, this, &UHealthComponent::DecreaseHealthOnTimer, _decreaseHealthInterval, true);
	}
}

void UHealthComponent::DecreaseHealthOnTimer()
{
	if(CurrentValue <= _decreaseHealthStopValue)
	{
		return;
	}

	Injure(Damage(_decreaseHealthAmount));
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

void UHealthComponent::Injure(const Damage damage)
{
	if (IsDead())
	{
		return;
	}

	auto const damageAmount = damage.GetAmount();
	auto const damageValue = -1 * FMath::Abs(damageAmount);
	_lastDamage = damage;
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

Damage UHealthComponent::GetLastDamage() const
{
	return _lastDamage;
}
