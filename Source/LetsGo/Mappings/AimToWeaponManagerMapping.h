#pragma once

#include "MappingComponent.h"
#include "LetsGo/AimProviders/AimProviderComponent.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"


#include "AimToWeaponManagerMapping.generated.h"

///<summary>
///Aim provider to weapon manager mapping. Persistent because aim provider can be changed
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAimToWeaponManagerMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UAimProviderComponent* _aimProviderComponent;

	UWeaponManagerComponent* _weaponManagerComponent;

	void OnAimProviderChanged() const;
};
