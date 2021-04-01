#pragma once

UENUM(BlueprintType)
enum class FSpawnPointType : uint8
{
	None UMETA(DisplayName = "None"),
	Player UMETA(DisplayName = "Player"),
	Weapon UMETA(DisplayName = "Weapon"),
	Health UMETA(DisplayName = "Health")
};
