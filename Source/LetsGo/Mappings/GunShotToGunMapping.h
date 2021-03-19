#pragma once

#include "MappingComponent.h"
#include "LetsGo/GunShots/GunShotComponent.h"
#include "LetsGo/WeaponSystem/IGun.h"

#include "GunShotToGunMapping.generated.h"

UCLASS(Abstract)
class LETSGO_API UGunShotToGunMapping : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

	virtual void Bind() const;
	
protected:
	IGun* Gun;

	AActor* GunActor;

	UGunShotComponent* GunShotComponent;

private:
	void OnWeaponInitialized(IWeapon* weapon);

	void OnGunInitialized(IGun* gun);

	bool _isWeaponDataSet = false;

	bool _isGunDataSet = false;

	void OnPartialInitialization();
};
