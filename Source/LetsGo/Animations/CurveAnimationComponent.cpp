#include "CurveAnimationComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "LetsGo/Logs/DevLogger.h"

const float UCurveAnimationComponent::DistanceErrorTolerance = 0.1f;

const float UCurveAnimationComponent::DistanceErrorToleranceSquared = DistanceErrorTolerance * DistanceErrorTolerance;

UCurveAnimationComponent::UCurveAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCurveAnimationComponent::Initialize(USceneComponent* moveableComponent, USceneComponent* moveableComponentRoot)
{
	_moveableComponent = moveableComponent;
	_moveableComponentRoot = moveableComponentRoot;
}

void UCurveAnimationComponent::SetState(const FCurveAnimationState state)
{
	if(_state == state)
	{
		return;
	}
	
	_state = state;
	_stateTime = 0;
}

void UCurveAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(!_moveableComponent || !_moveableComponentRoot)
	{
		return;
	}
	
	switch (_state)
	{
		case FCurveAnimationState::None:
			break;

		case FCurveAnimationState::ApplyCurve:
			ProcessApplyCurveState(DeltaTime);
			break;
		
		case FCurveAnimationState::MoveToOrigin:
			ProcessMoveToOriginState(DeltaTime);
			break;
		default:
			DevLogger::GetLoggingChannel()->LogValue("Unhandled state:", static_cast<int>(_state), LogSeverity::Error);
			break;
	}
	
	_stateTime += DeltaTime;
}

void UCurveAnimationComponent::ProcessApplyCurveState(const float deltaTime)
{
	if(!(_curveForward || _curveRight || _curveUp))
	{
		return;
	}

	auto offset = FVector::ZeroVector;
	
	if (_curveForward)
	{
		auto const forwardOffset = GetLocationOffset(_moveableComponentRoot->GetForwardVector(), _curveForward);
		offset += forwardOffset;
	}

	if (_curveRight)
	{
		auto const rightOffset = GetLocationOffset(_moveableComponentRoot->GetRightVector(), _curveRight);
		offset += rightOffset;
	}

	if (_curveUp)
	{
		auto const upOffset = GetLocationOffset(_moveableComponentRoot->GetUpVector(), _curveUp);
		offset += upOffset;
	}
	
	auto const rootLocation = _moveableComponentRoot->GetComponentLocation();
	auto const targetLocation = rootLocation + offset;
	
	_moveableComponent->SetWorldLocation(targetLocation);
}

void UCurveAnimationComponent::ProcessMoveToOriginState(const float deltaTime) const
{
	auto const currentLocation = _moveableComponent->GetComponentLocation();
	auto const rootLocation = _moveableComponentRoot->GetComponentLocation();
	auto const distanceSquared = FVector::DistSquared(currentLocation, rootLocation);
	if (distanceSquared < DistanceErrorToleranceSquared)
	{
		return;
	}
	auto const lerpedLocation = UKismetMathLibrary::VLerp(currentLocation, rootLocation, deltaTime * _moveToOriginLerpSpeed);
	
	_moveableComponent->SetWorldLocation(lerpedLocation);
}

FVector UCurveAnimationComponent::GetLocationOffset(const FVector& direction, const UCurveFloat* curve)
{
	auto const offsetAmount = GetOffsetAmount(curve);
	auto const offset = direction * offsetAmount;
	return offset;
}

float UCurveAnimationComponent::GetOffsetAmount(const UCurveFloat* curve)
{
	if(!curve)
	{
		return 0;
	}
	
	curve->GetTimeRange(_bufferMinTime, _bufferMaxTime);

	auto const scaledTime = _stateTime * _curveTimeMultiplier;
	auto const loopedScaledTime = FMath::Fmod(scaledTime, _bufferMaxTime);
	auto const curveValue = curve->GetFloatValue(loopedScaledTime);
	auto const offset = curveValue * _curveMagnitudeMultiplier;
	
	return offset;
}
