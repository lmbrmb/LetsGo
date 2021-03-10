#pragma once

UENUM(BlueprintType)
enum class FMatchHighlight : uint8
{
	None UMETA(DisplayName = "None"),
	FirstBlood UMETA(DisplayName = "First blood"),
	DoubleKill UMETA(DisplayName = "Double kill"),
	Impressive UMETA(DisplayName = "Impressive"),
};