#include "GunRunAnimationMapping.h"

#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

UGunRunAnimationMapping::UGunRunAnimationMapping()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGunRunAnimationMapping::Map()
{
	AssertIsTrue(_curveUp || _curveRight || _curveForward);
	AssertNameIsAssigned(_animationId);

	auto const owner = GetOwner();
	
	TArray<UCurveAnimationBlenderComponent*> curveAnimationBlenderComponents;
	owner->GetComponents<UCurveAnimationBlenderComponent>(curveAnimationBlenderComponents);

	auto matchCount = 0;
	for (auto curveAnimationBlenderComponent : curveAnimationBlenderComponents)
	{
		if (curveAnimationBlenderComponent->GetBlenderId() == _blenderId)
		{
			matchCount++;
			_curveAnimationBlenderComponent = curveAnimationBlenderComponent;
		}
	}

	AssertIsNotNull(_curveAnimationBlenderComponent);
	AssertIsEqual(matchCount, 1);

	_movementComponent = owner->FindComponentByClass<UMovementComponentBase>();
	AssertIsNotNull(_movementComponent);

	SetupAnimation();
	
	_isInitialized = true;
}

void UGunRunAnimationMapping::SetupAnimation() const
{
	_curveAnimationBlenderComponent->AddAnimation(
		_animationId,
		_animationLoop,
		_isAnimationEnabled,
		_curveMagnitudeMultiplier,
		_curveTimeMultiplier
	);
	
	if (_curveUp)
	{
		_curveAnimationBlenderComponent->AddCurve(_animationId, _curveUp, FVector::UpVector);
	}

	if (_curveRight)
	{
		_curveAnimationBlenderComponent->AddCurve(_animationId, _curveRight, FVector::RightVector);
	}

	if (_curveForward)
	{
		_curveAnimationBlenderComponent->AddCurve(_animationId, _curveForward, FVector::ForwardVector);
	}
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
	auto const isEnabled = isMoving && !isInAir;

	if(_isEnabled != isEnabled)
	{
		_isEnabled = isEnabled;
		_curveAnimationBlenderComponent->EnableAnimation(_animationId, _isEnabled);
	}
}
