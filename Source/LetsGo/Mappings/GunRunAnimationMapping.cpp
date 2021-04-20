#include "GunRunAnimationMapping.h"

#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

UGunRunAnimationMapping::UGunRunAnimationMapping()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGunRunAnimationMapping::Map()
{
	AssertNameIsAssigned(_animationId);

	auto const owner = GetOwner();
	
	TArray<UCurveAnimationComponent*> curveAnimationComponents;
	owner->GetComponents<UCurveAnimationComponent>(curveAnimationComponents);

	auto matchCount = 0;
	for (auto curveAnimationComponent : curveAnimationComponents)
	{
		if (curveAnimationComponent->GetAnimationId() == _animationId)
		{
			matchCount++;
			_curveAnimationComponent = curveAnimationComponent;
		}
	}

	AssertIsNotNull(_curveAnimationComponent);
	AssertIsEqual(matchCount, 1);

	_movementComponent = owner->FindComponentByClass<UMovementComponentBase>();
	AssertIsNotNull(_movementComponent);
	
	_isInitialized = true;
}

void UGunRunAnimationMapping::TickComponent(
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
