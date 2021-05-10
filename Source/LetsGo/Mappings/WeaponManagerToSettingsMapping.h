#pragma once

#include "MappingComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

#include "WeaponManagerToSettingsMapping.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UWeaponManagerToSettingsMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UWeaponManagerComponent* _weaponManagerComponent = nullptr;

	AMatchGameMode* _matchGameMode;

	void OnWeaponManagerInitialized() const;

	void OnWeaponPivotChanged(const int weaponPivotIndex) const;
};
