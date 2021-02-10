#include "HealthComponent.h"

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	MaximizeCurrentValue();
}

void UHealthComponent::Kill()
{
	MinimizeCurrentValue();
}

bool UHealthComponent::IsAlive() const
{
	return CurrentValue > MinValue;
}
