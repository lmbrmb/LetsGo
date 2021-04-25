#pragma once

UENUM(BlueprintType)
enum class FLeadState : uint8
{
	None UMETA(DisplayName = "None"),
	Taken UMETA(DisplayName = "Taken"),
	Tied UMETA(DisplayName = "Tied"),
	Lost UMETA(DisplayName = "Lost"),
};
