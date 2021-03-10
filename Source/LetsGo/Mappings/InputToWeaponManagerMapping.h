#pragma once

#include "MappingComponent.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

#include "InputToWeaponManagerMapping.generated.h"

///<summary>
///Maps user input to UWeaponManagerComponent methods
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UInputToWeaponManagerMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

	virtual bool ShouldDestroyAfterMapping() const override;

private:
	int _lastDpadHorizontalAxisValue = 0;

	UWeaponManagerComponent* _weaponManagerComponent = nullptr;
	
	void ChangeWeaponDpad(float indexModifier);
};
