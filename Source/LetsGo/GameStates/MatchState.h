#pragma once

UENUM(BlueprintType)
enum class MatchState : uint8
{
	None UMETA(DisplayName = "None"),
	WarmUp UMETA(DisplayName = "Warm up"),
	Started UMETA(DisplayName = "Started"),
	Ended UMETA(DisplayName = "Ended")
};
