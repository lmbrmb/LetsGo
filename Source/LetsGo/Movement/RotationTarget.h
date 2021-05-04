#pragma once

UENUM(BlueprintType)
enum class RotationTarget : uint8
{
	None UMETA(DisplayName = "None"),
	Location UMETA(DisplayName = "Location"),
	MovementDirection UMETA(DisplayName = "Movement direction")
};
