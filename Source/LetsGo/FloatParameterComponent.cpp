#ifdef DEBUG
#endif

#include "FloatParameterComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Logs/DevLogger.h"

UFloatParameterComponent::UFloatParameterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFloatParameterComponent::Init(float maxValue, float currentValue, float minValue)
{
	MinValue = minValue;
	MaxValue = maxValue;

	if(minValue > maxValue)
	{
		DevLogger::GetLoggingChannel()->Log(
			"Min value " + UKismetStringLibrary::Conv_FloatToString(minValue)
			+ " is more than max value " + UKismetStringLibrary::Conv_FloatToString(maxValue),
			LogSeverity::Error
		);
	}
	
	if(currentValue < minValue)
	{
		DevLogger::GetLoggingChannel()->Log(
			"Current value " + UKismetStringLibrary::Conv_FloatToString(currentValue)
			+ " is less than min value " + UKismetStringLibrary::Conv_FloatToString(minValue),
			LogSeverity::Warning
		);
	}

	if (currentValue > maxValue)
	{
		DevLogger::GetLoggingChannel()->Log(
			"Current value " + UKismetStringLibrary::Conv_FloatToString(currentValue)
			+ " is more than max value " + UKismetStringLibrary::Conv_FloatToString(maxValue),
			LogSeverity::Warning
		);
	}
	
	SetCurrentValue(currentValue);
}

void UFloatParameterComponent::SetCurrentValue(float currentValue)
{
	auto const previous = CurrentValue;
	CurrentValue = FMath::Max(MinValue, FMath::Min(MaxValue, currentValue));
	if(previous != CurrentValue)
	{
		Changed.Broadcast();
	}
}

void UFloatParameterComponent::SetMinValue(float minValue)
{
	MinValue = minValue;
	SetCurrentValue(CurrentValue);
}

void UFloatParameterComponent::SetMaxValue(float maxValue)
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

void UFloatParameterComponent::ChangeValue(float amount)
{
	SetCurrentValue(CurrentValue + amount);
}
