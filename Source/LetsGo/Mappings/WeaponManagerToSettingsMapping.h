#pragma once

#include "MappingComponent.h"
#include "LetsGo/Settings/PlayerSettingsManager.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

#include "WeaponManagerToSettingsMapping.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UWeaponManagerToSettingsMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _weaponPivotIndexSettingName = "WeaponPivotIndex";

	UWeaponManagerComponent* _weaponManagerComponent = nullptr;

	PlayerSettingsManager* _playerSettingsManager = nullptr;
	
	void OnWeaponManagerInitialized() const;

	void OnWeaponPivotChanged(const int weaponPivotIndex) const;
};
