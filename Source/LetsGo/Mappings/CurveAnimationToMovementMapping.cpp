#include "CurveAnimationToMovementMapping.h"

#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

UCurveAnimationToMovementMapping::UCurveAnimationToMovementMapping()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCurveAnimationToMovementMapping::Map()
{
	auto const owner = GetOwner();

	_movementComponent = owner->FindComponentByClass<UMovementComponentBase>();
	AssertIsNotNull(_movementComponent);

	_curveAnimationComponent = owner->FindComponentByClass<UCurveAnimationComponent>();
	AssertIsNotNull(_curveAnimationComponent);

	_isInitialized = true;
}

void UCurveAnimationToMovementMapping::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!_isInitialized)
	{
		return;
	}

	auto const isMoving = _movementComponent->GetAbsoluteMovementAmount() > 0;
	auto const isInAir = _movementComponent->GetIsInAir();
	
	auto const state = isMoving && !isInAir ? FCurveAnimationState::ApplyCurve : FCurveAnimationState::MoveToOrigin;

	if(_state != state)
	{
		_state = state;
		_curveAnimationComponent->SetState(_state);
	}
}
