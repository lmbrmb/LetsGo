#pragma once

#include "MappingComponent.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Movement/ThirdPersonMovementComponent.h"

#include "InputToTpMovementMapping.generated.h"

///<summary>
///Maps user input to UThirdPersonMovementComponent method calls
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UInputToTpMovementMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UInputComponent* _inputComponent = nullptr;

	UThirdPersonMovementComponent* _thirdPersonMovementComponent = nullptr;

	void OnOwnerDied(UHealthComponent* healthComponent, float delta) const;

	void Bind();

	void Unbind() const;

	void OnSprintPressed();

	void OnSprintReleased();

	void OnWalkPressed();

	void OnWalkReleased();
};
