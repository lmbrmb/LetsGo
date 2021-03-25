#pragma once

#include "MappingComponent.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Movement/FirstPersonMovementComponent.h"

#include "InputToFpMovementMapping.generated.h"

///<summary>
///Maps user input to UFirstPersonMovementComponent method calls. Persistent
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UInputToFpMovementMapping final : public UMappingComponent
{
	GENERATED_BODY()
	
protected:
	virtual void Map() override;

private:
	UInputComponent* _inputComponent = nullptr;

	UFirstPersonMovementComponent* _firstPersonMovementComponent = nullptr;

	void OnOwnerDied(const UHealthComponent*, float delta) const;
	
	void Bind() const;

	void Unbind() const;
};
