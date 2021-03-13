#include "FloatParameterComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Logs/DevLogger.h"
#include "Utils/AssertUtils.h"

UFloatParameterComponent::UFloatParameterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFloatParameterComponent::BeginPlay()
{
	AssertIsLessOrEqual(MinValue, MaxValue);
	AssertIsLessOrEqual(InitialValue, MaxValue);
	AssertIsGreaterOrEqual(InitialValue, MinValue);

	SetCurrentValue(InitialValue);
	
	Init();
}

void UFloatParameterComponent::Init()
{
	//Do nothing
}

void UFloatParameterComponent::SetCurrentValue(const float currentValue)
{
	auto const previous = CurrentValue;
	CurrentValue = FMath::Max(MinValue, FMath::Min(MaxValue, currentValue));
	auto const delta = CurrentValue - previous;
	if(!FMath::IsNearlyZero(delta))
	{
		OnChanged(delta);
	}
}

void UFloatParameterComponent::SetMinValue(const float minValue)
{
	MinValue = minValue;
	SetCurrentValue(CurrentValue);
}

void UFloatParameterComponent::SetMaxValue(const float maxValue)
{
	MaxValue = maxValue;
	SetCurrentValue(CurrentValue);
}

void UFloatParameterComponent::MinimizeCurrentValue()
{
	CurrentValue = MinValue;
}

void UFloatParameterComponent::MaximizeCurrentValue()
{
	CurrentValue = MaxValue;
}

float UFloatParameterComponent::GetMinValue() const
{
	return MinValue;
}

float UFloatParameterComponent::GetMaxValue() const
{
	return MaxValue;
}

float UFloatParameterComponent::GetCurrentValue() const
{
	return CurrentValue;
}

float UFloatParameterComponent::GetNormalizedValue() const
{
	auto const current = CurrentValue - MinValue;
	auto const max = MaxValue - MinValue;
	if (FMath::IsNearlyZero(max))
		return 0;
	return current / max;
}

void UFloatParameterComponent::ChangeValue(const float amount)
{
	SetCurrentValue(CurrentValue + amount);
}
