#include "BotMovementComponent.h"

bool UBotMovementComponent::IsTargetLocationValid() const
{
	return _isTargetLocationValid;
}

const FVector& UBotMovementComponent::GetTargetLocation() const
{
	return _targetLocation;
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

FVector UBotMovementComponent::GetMovementDirection()
{
	if (!_isTargetLocationValid)
	{
		return FVector::ZeroVector;
	}
	auto const delta = _targetLocation - RootCollider->GetComponentLocation();
	auto direction = delta.GetSafeNormal();
	direction = FVector::VectorPlaneProject(direction, FVector::UpVector);

	return direction.GetSafeNormal();
}

float UBotMovementComponent::GetMovementSpeed()
{
	auto const forwardVector = RootCollider->GetForwardVector();
	auto const dotForward = FVector::DotProduct(forwardVector, _inputMovementDirection);
	auto const isMovingForward = dotForward >= MIN_DOT_FORWARD;
	auto const speed = isMovingForward ? _actorMoveForwardSpeed : _actorMoveBackwardSpeed;
	auto const environmentMultiplier = GetIsInAir() ? _airMultiplier : 1.0f;
	auto const movementSpeed = speed * environmentMultiplier;
	return movementSpeed;
}

float UBotMovementComponent::GetAbsoluteMovementAmount() const
{
	return _isTargetLocationValid ? 1 : 0;
}

void UBotMovementComponent::Init(AActor* actor)
{
	//Do nothing
}

void UBotMovementComponent::ProcessInput()
{
	//Do nothing
}

void UBotMovementComponent::ResetInput()
{
	//Do nothing
}

void UBotMovementComponent::CustomTick(float deltaTime)
{
	//Do nothing
}
