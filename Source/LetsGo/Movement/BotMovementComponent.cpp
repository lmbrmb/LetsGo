#include "BotMovementComponent.h"

#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "LetsGo/Logs/DevLogger.h"

const FVector& UBotMovementComponent::GetTargetLocation() const
{
	return _targetLocation;
}

bool UBotMovementComponent::IsTargetLocationValid() const
{
	return _isTargetLocationValid;
}

void UBotMovementComponent::SetTargetLocation(const FVector& targetLocation)
{
	_targetLocation = targetLocation;
	_isTargetLocationValid = true;
}

void UBotMovementComponent::ClearTargetLocation()
{
	_isTargetLocationValid = false;
}

const FVector& UBotMovementComponent::GetTargetRotation() const
{
	return _targetRotationLocation;
}

void UBotMovementComponent::SetTargetRotationAsLocation(const FVector& targetRotationLocation)
{
	_targetRotationLocation = targetRotationLocation;
	_rotationTarget = RotationTarget::Location;
}

void UBotMovementComponent::SetTargetRotationAsMovementDirection()
{
	_rotationTarget = RotationTarget::MovementDirection;
}

void UBotMovementComponent::ClearTargetRotation()
{
	_rotationTarget = RotationTarget::None;
}

const FVector& UBotMovementComponent::GetMovementDirection() const
{
	return _inputMovementDirection;
}

float UBotMovementComponent::GetBaseMovementSpeed()
{
	auto const forwardVector = RootCollider->GetForwardVector();
	auto const dotForward = FVector::DotProduct(forwardVector, _inputMovementDirection);
	auto const isMovingForward = dotForward >= MIN_DOT_FORWARD;
	auto const speed = isMovingForward ? _actorMoveForwardSpeed : _actorMoveBackwardSpeed;
	return speed;
}

float UBotMovementComponent::GetAbsoluteMovementAmount() const
{
	return _isTargetLocationValid ? 1 : 0;
}

void UBotMovementComponent::ProcessInput()
{
	if (!_isTargetLocationValid)
	{
		_inputMovementDirection = FVector::ZeroVector;
		return;
	}

	CalculateNextLocation();

	if(!_isNextLocationValid)
	{
		_inputMovementDirection = FVector::ZeroVector;
		return;
	}
	
	auto const delta = _nextLocation - RootCollider->GetComponentLocation();
	auto direction = delta.GetSafeNormal();
	direction = FVector::VectorPlaneProject(direction, FVector::UpVector);

	_inputMovementDirection = direction.GetSafeNormal();
}

void UBotMovementComponent::CalculateNextLocation()
{
	if(!_isTargetLocationValid)
	{
		_isNextLocationValid = false;
		return;
	}
	
	auto const selfLocation = GetRootColliderLocation();
	auto const world = GetWorld();
	auto const navigationSystemV1 = UNavigationSystemV1::GetCurrent(world);
	auto const navigationPath = navigationSystemV1->FindPathToLocationSynchronously(world, selfLocation, _targetLocation);

	if (!navigationPath)
	{
		_isNextLocationValid = false;
		return;
	}

	auto const navigationPathPointsCount = navigationPath->PathPoints.Num();
	
	if (navigationPathPointsCount < 2)
	{
		/*DevLogger::GetLoggingChannel()->LogValue(
			"Navigation path points count is invalid. Points count:",
			navigationPathPointsCount,
			LogSeverity::Error
		);*/

		_isNextLocationValid = false;
		return;
	}
	
	_nextLocation = navigationPath->PathPoints[1];
	_isNextLocationValid = true;
}

void UBotMovementComponent::CustomTick(const float deltaTime)
{
	ProcessTargetRotation(deltaTime);
}

void UBotMovementComponent::ProcessTargetRotation(const float deltaTime) const
{
	FVector rotationDirection;
	switch (_rotationTarget)
	{
		case RotationTarget::Location:
			rotationDirection = (_targetRotationLocation - GetRootColliderLocation()).GetSafeNormal();
			rotationDirection = FVector::VectorPlaneProject(rotationDirection, FVector::UpVector).GetSafeNormal();;
			break;
		case RotationTarget::MovementDirection:
			rotationDirection = _inputMovementDirection;
			break;
		case RotationTarget::None:
			return;
		default:
			DevLogger::GetLoggingChannel()->LogValue("Unknown RotationTarget state", static_cast<int>(_rotationTarget));
			return;
	}
	ProcessActorRotation(deltaTime, rotationDirection);
}

void UBotMovementComponent::Init(AActor* actor)
{
	//Do nothing
}

void UBotMovementComponent::ResetInput()
{
	//Do nothing
}
