#include "FirstPersonMovementComponent.h"

const FVector& UFirstPersonMovementComponent::GetMovementDirection() const
{
	return _inputMovementDirection;
}

float UFirstPersonMovementComponent::GetBaseMovementSpeed()
{
	auto const forwardVector = RootCollider->GetForwardVector();
	auto const dotForward = FVector::DotProduct(forwardVector, _inputMovementDirection);
	auto const isMovingForward = dotForward >= MIN_DOT_FORWARD;
	auto const speed = isMovingForward ? _actorMoveForwardSpeed : _actorMoveBackwardSpeed;
	auto const movementSpeed = speed * _absoluteMovementAmount;
	return movementSpeed;
}

void UFirstPersonMovementComponent::CustomTick(const float deltaTime)
{
	ProcessCameraPitch(deltaTime);
	ProcessActorYaw(deltaTime);
}

void UFirstPersonMovementComponent::ProcessCameraPitch(const float deltaTime) const
{
	auto const hasPitchInput = FMath::Abs(_cameraPitchInputAmount) > MIN_ROTATION_INPUT_AMOUNT;

	if (!hasPitchInput)
	{
		return;
	}

	auto rotation = _cameraComponent->GetRelativeRotation();
	rotation.Pitch = ClampCameraPitch(rotation.Pitch + _cameraPitchInputAmount * _cameraPitchSpeed * deltaTime);
	_cameraComponent->SetRelativeRotation(rotation);
}

void UFirstPersonMovementComponent::ProcessActorYaw(float const deltaTime) const
{
	auto const hasYawInput = FMath::Abs(_actorYawInputAmount) > MIN_ROTATION_INPUT_AMOUNT;

	if (!hasYawInput)
	{
		return;
	}

	auto const yawDelta = _actorYawInputAmount * _actorYawSpeed * deltaTime;
	RootCollider->AddRelativeRotation(FRotator(0, yawDelta, 0));
}

void UFirstPersonMovementComponent::Init(AActor* actor)
{
	_cameraComponent = actor->FindComponentByClass<UCameraComponent>();
}

void UFirstPersonMovementComponent::ProcessInput()
{
	auto const hasForwardMovementInput = FMath::Abs(_actorForwardMovementInputAmount) > MIN_MOVEMENT_INPUT;
	auto const hasRightMovementInput = FMath::Abs(_actorRightMovementInputAmount) > MIN_MOVEMENT_INPUT;

	if (!hasForwardMovementInput && !hasRightMovementInput)
	{
		_inputMovementDirection = FVector::ZeroVector;
		_absoluteMovementAmount = 0;
		return;
	}

	auto absoluteMovementAmount = FMath::Abs(_actorForwardMovementInputAmount) + FMath::Abs(_actorRightMovementInputAmount);
	if (absoluteMovementAmount > 1)
	{
		absoluteMovementAmount = 1;
	}
	_absoluteMovementAmount = absoluteMovementAmount;

	auto direction = FVector::ZeroVector;
	auto const rootForward = RootCollider->GetForwardVector();
	auto const rootRight = RootCollider->GetRightVector();
	
	if (hasForwardMovementInput)
	{
		direction += rootForward * _actorForwardMovementInputAmount;
	}

	if (hasRightMovementInput)
	{
		direction += rootRight * _actorRightMovementInputAmount;
	}

	_inputMovementDirection = direction.GetSafeNormal();
}

void UFirstPersonMovementComponent::ResetInput()
{
	_actorForwardMovementInputAmount = 0;
	_actorRightMovementInputAmount = 0;
	_cameraPitchInputAmount = 0;
	_actorYawInputAmount = 0;
}

float UFirstPersonMovementComponent::GetAbsoluteMovementAmount() const
{
	return _absoluteMovementAmount;
}

void UFirstPersonMovementComponent::SetLookSpeed(
	const float cameraPitchSpeed,
	const float actorYawSpeed
)
{
	_cameraPitchSpeed = cameraPitchSpeed;
	_actorYawSpeed = actorYawSpeed;
}

void UFirstPersonMovementComponent::AddActorForwardMovementInput(const float amount)
{
	_actorForwardMovementInputAmount += amount;
}

void UFirstPersonMovementComponent::AddActorRightMovementInput(const float amount)
{
	_actorRightMovementInputAmount += amount;
}

void UFirstPersonMovementComponent::AddActorYawInput(const float amount)
{
	_actorYawInputAmount += amount;
}

void UFirstPersonMovementComponent::AddCameraPitchInput(const float amount)
{
	_cameraPitchInputAmount += amount;
}

float UFirstPersonMovementComponent::ClampCameraPitch(float pitch) const
{
	if (pitch > 0)
	{
		if (pitch > _cameraPitchMax)
		{
			pitch = _cameraPitchMax;
		}
	}
	else
	{
		if (pitch < _cameraPitchMin)
		{
			pitch = _cameraPitchMin;
		}
	}
	return pitch;
}
