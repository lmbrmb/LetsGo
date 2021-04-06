#include "BotMovementComponent.h"

bool UBotMovementComponent::IsTargetMovementLocationValid() const
{
	return _isTargetMovementLocationValid;
}

const FVector& UBotMovementComponent::GetTargetLocation() const
{
	return _targetMovementLocation;
}

void UBotMovementComponent::SetTargetMovementLocation(const FVector& targetMovementLocation)
{
	_targetMovementLocation = targetMovementLocation;
	_isTargetMovementLocationValid = true;
}

void UBotMovementComponent::ClearTargetMovementLocation()
{
	_isTargetMovementLocationValid = false;
}

FVector UBotMovementComponent::GetMovementDirection()
{
	return _inputMovementDirection;
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
	return _isTargetMovementLocationValid ? 1 : 0;
}


void UBotMovementComponent::ProcessInput()
{
	if (!_isTargetMovementLocationValid)
	{
		_inputMovementDirection = FVector::ZeroVector;
		return;
	}
	
	auto const delta = _targetMovementLocation - RootCollider->GetComponentLocation();
	auto direction = delta.GetSafeNormal();
	direction = FVector::VectorPlaneProject(direction, FVector::UpVector);

	_inputMovementDirection = direction.GetSafeNormal();
}

void UBotMovementComponent::CustomTick(const float deltaTime)
{
	ProcessRotation();
}

void UBotMovementComponent::ProcessRotation()
{
}

void UBotMovementComponent::Init(AActor* actor)
{
	//Do nothing
}

void UBotMovementComponent::ResetInput()
{
	//Do nothing
}
