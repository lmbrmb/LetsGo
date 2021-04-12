#include "BotMovementComponent.h"

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

bool UBotMovementComponent::IsTargetRotationValid() const
{
	return _isTargetRotationLocationValid;
}

void UBotMovementComponent::SetTargetRotation(const FVector& targetRotationLocation)
{
	_targetRotationLocation = targetRotationLocation;
	_isTargetRotationLocationValid = true;
}

void UBotMovementComponent::ClearTargetRotation()
{
	_isTargetRotationLocationValid = false;
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
	
	auto const delta = _targetLocation - RootCollider->GetComponentLocation();
	auto direction = delta.GetSafeNormal();
	direction = FVector::VectorPlaneProject(direction, FVector::UpVector);

	_inputMovementDirection = direction.GetSafeNormal();
}

void UBotMovementComponent::CustomTick(const float deltaTime)
{
	ProcessTargetRotation(deltaTime);
}

void UBotMovementComponent::ProcessTargetRotation(const float deltaTime) const
{
	if (!_isTargetRotationLocationValid)
	{
		return;
	}

	auto rotationDirection = (_targetRotationLocation - GetRootColliderLocation()).GetSafeNormal();
	rotationDirection = FVector::VectorPlaneProject(rotationDirection, FVector::UpVector).GetSafeNormal();;
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
