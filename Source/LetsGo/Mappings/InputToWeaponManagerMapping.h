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
	int _lastDpadHorizontalAxisValue = 0;

	UWeaponManagerComponent* _weaponManagerComponent = nullptr;

	UInputComponent* _inputComponent = nullptr;

	void ChangeWeaponDpad(const float rawAxisValue);

	void OnOwnerDied(UHealthComponent* healthComponent, float delta) const;

	void Bind();

	void Unbind() const;
};
