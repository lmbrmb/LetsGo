#include "FloatParameterComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Logs/DevLogger.h"

UFloatParameterComponent::UFloatParameterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UFloatParameterComponent::BeginPlay()
{
	Init();
}

void UFloatParameterComponent::Init()
{
	if(MinValue > MaxValue)
	{
		DevLogger::GetLoggingChannel()->Log(
			"Min value " + UKismetStringLibrary::Conv_FloatToString(MinValue)
			+ " is more than max value " + UKismetStringLibrary::Conv_FloatToString(MaxValue),
			LogSeverity::Error
		);
	}
	
	if(InitialValue < MinValue)
	{
		DevLogger::GetLoggingChannel()->Log(
			"Current value " + UKismetStringLibrary::Conv_FloatToString(InitialValue)
			+ " is less than min value " + UKismetStringLibrary::Conv_FloatToString(MinValue),
			LogSeverity::Warning
		);
	}

	if (InitialValue > MaxValue)
	{
		DevLogger::GetLoggingChannel()->Log(
			"Current value " + UKismetStringLibrary::Conv_FloatToString(InitialValue)
			+ " is more than max value " + UKismetStringLibrary::Conv_FloatToString(MaxValue),
			LogSeverity::Warning
		);
	}
	
	SetCurrentValue(InitialValue);
}

void UFloatParameterComponent::SetCurrentValue(float currentValue)
{
	auto const previous = CurrentValue;
	CurrentValue = FMath::Max(MinValue, FMath::Min(MaxValue, currentValue));
	if(previous != CurrentValue)
	{
		OnChanged();
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
