#pragma once

#include "MappingComponent.h"
#include "LetsGo/GunShots/GunShotComponent.h"
#include "LetsGo/WeaponSystem/IGun.h"

#include "GunShotToGunMapping.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UGunShotToGunMapping : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	IGun* _gun;

	AActor* _gunActor;

	UGunShotComponent* _gunShotComponent;

	void OnWeaponInitialized(IWeapon* weapon);

	void OnGunInitialized(IGun* gun);

	bool _isWeaponDataSet = false;

	bool _isGunDataSet = false;

	void OnPartialInitialization();

	void BindOnShotPerformed() const;
};
