#pragma once

UENUM(BlueprintType)
enum class FFirePivotMode : uint8
{
	AlwaysFirst UMETA(DisplayName = "Always first"),
	Cycle UMETA(DisplayName = "Cycle"),
	AllSimultaneously UMETA(DisplayName = "All simultaneously"),
};
