#pragma once

UENUM(BlueprintType)
enum class FMedalType : uint8
{
	None UMETA(DisplayName = "None"),
	FirstBlood UMETA(DisplayName = "First blood"),
	Excellent UMETA(DisplayName = "Excellent"),
	Impressive UMETA(DisplayName = "Impressive"),
};
