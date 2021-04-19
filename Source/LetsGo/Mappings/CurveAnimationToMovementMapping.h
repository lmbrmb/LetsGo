#pragma once

#include "MappingComponent.h"

#include "LetsGo/Movement/MovementComponentBase.h"
#include "LetsGo/Animations/CurveAnimationComponent.h"

#include "CurveAnimationToMovementMapping.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UCurveAnimationToMovementMapping : public UMappingComponent
{
	GENERATED_BODY()

public:
	UCurveAnimationToMovementMapping();

protected:
	virtual void Map() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UMovementComponentBase* _movementComponent = nullptr;

	UCurveAnimationComponent* _curveAnimationComponent = nullptr;

	bool _isInitialized = false;

	FCurveAnimationState _state;
};
