#pragma once

UENUM(BlueprintType)
enum class FEnvironment : uint8
{
	None UMETA(DisplayName = "None"),
	Ground UMETA(DisplayName = "Ground"),
	Water UMETA(DisplayName = "Water")
};
