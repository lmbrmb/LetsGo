#pragma once

UENUM(BlueprintType)
enum class MovementSpeedState : uint8
{
	Run UMETA(DisplayName = "Run"),
	Walk UMETA(DisplayName = "Walk"),
	Sprint UMETA(DisplayName = "Sprint"),
};
