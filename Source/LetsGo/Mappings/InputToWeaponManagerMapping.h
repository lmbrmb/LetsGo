#pragma once

#include "MappingComponent.h"

#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

#include "InputToWeaponManagerMapping.generated.h"

///<summary>
///Maps user input to UWeaponManagerComponent methods. Persistent
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UInputToWeaponManagerMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _machinegunId = "MachinegunQ3";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _shotgunId = "ShotgunQ3";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _rocketLauncherId = "RocketLauncherQ3";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _railgunId = "RailgunQ3";

	int _lastDpadHorizontalAxisValue = 0;

	UWeaponManagerComponent* _weaponManagerComponent = nullptr;

	UInputComponent* _inputComponent = nullptr;

	void OnChangeWeaponDpad(const float rawAxisValue);

	void OnEquipMachinegun();

	void OnEquipShotgun();

	void OnEquipRocketLauncher();

	void OnEquipRailgun();

	void OnOwnerDied(UHealthComponent* healthComponent, float delta) const;

	void Bind();

	void Unbind() const;
};
