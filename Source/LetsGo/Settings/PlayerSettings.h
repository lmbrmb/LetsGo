#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSettings.generated.h"

UCLASS()
class LETSGO_API UPlayerSettings : public USaveGame
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Weapon pivot index. 0 - right, 1 - center
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	int WeaponPivotIndex;

	/// <summary>
	/// Player skin id.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	FName PlayerSkinId;

	/// <summary>
	/// Mouse sensitivity modifier. Range: 1-10
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	int MouseSensitivity;

	/// <summary>
	/// Should equip weapon on pickup. true / false
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	bool ShouldEquipWeaponOnPickup;
};
