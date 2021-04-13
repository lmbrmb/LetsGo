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

	virtual void Bind() const;
	
protected:
	IGun* Gun;

	AActor* GunActor;

	UGunShotComponent* GunShotComponent;

private:
	void OnWeaponInitialized();

	void OnGunInitialized();

	bool _isWeaponDataSet = false;

	bool _isGunDataSet = false;

	void OnPartialInitialization();
};
