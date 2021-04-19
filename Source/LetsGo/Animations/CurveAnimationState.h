#pragma once

UENUM(BlueprintType)
enum class FCurveAnimationState : uint8
{
	None UMETA(DisplayName = "None"),
	ApplyCurve UMETA(DisplayName = "Apply curve"),
	MoveToOrigin UMETA(DisplayName = "Move to origin")
};
