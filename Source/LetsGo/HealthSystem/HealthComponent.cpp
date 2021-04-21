#include "HealthComponent.h"

#include "LetsGo/Utils/AssertUtils.h"

void UHealthComponent::OnChanged(const float delta)
{
	HealthChanged.Broadcast(this, delta);
	
	if(IsDead())
	{
		Died.Broadcast(this, delta);
	}
}

void UHealthComponent::Init()
{
	AssertIsGreaterOrEqual(_decreaseHealthInterval, 0.0f);
	AssertIsGreaterOrEqual(_decreaseHealthAmount, 0.0f);
	AssertIsGreaterOrEqual(_maxNormalHealth, 0.0f);
	
	if(_decreaseHealthAmount > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(_decreaseHealthTimerHandle, this, &UHealthComponent::DecreaseHealthOnTimer, _decreaseHealthInterval, true);
	}
}

void UHealthComponent::BeginDestroy()
{
	Super::BeginDestroy();

	if(_decreaseHealthTimerHandle.IsValid())
	{
		auto const world = GetWorld();
		if(world)
		{
			world->GetTimerManager().ClearTimer(_decreaseHealthTimerHandle);
		}
	}
}

void UHealthComponent::DecreaseHealthOnTimer()
{
	if(CurrentValue <= _maxNormalHealth)
	{
		return;
	}

	TryInjure(Damage(_decreaseHealthAmount));
}

bool UHealthComponent::TryHeal(const float healAmount)
{
	if(IsDead())
	{
		return false;
	}

	auto const healValue = FMath::Abs(healAmount);
	ChangeValue(healValue);
	return true;
}

bool UHealthComponent::TryInjure(const Damage& damage)
{
	if (IsDead())
	{
		return false;
	}

	auto const damageAmount = damage.GetAmount();
	auto const damageValue = -1 * FMath::Abs(damageAmount);
	_lastDamage = damage;
	ChangeValue(damageValue);
	return true;
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

float UHealthComponent::GetMaxNormalHealth() const
{
	return _maxNormalHealth;
}

const Damage& UHealthComponent::GetLastDamage() const
{
	return _lastDamage;
}
