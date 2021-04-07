#include "ThirdPersonMovementComponent.h"
#include "LetsGo/Utils/FVectorUtils.h"

float UThirdPersonMovementComponent::GetAbsoluteMovementAmount() const
{
	return _absoluteMovementAmount;
}

FVector UThirdPersonMovementComponent::GetMovementDirection()
{
	return _inputMovementDirection;
}

float UThirdPersonMovementComponent::GetMovementSpeed()
{
	return _movementSpeed;
}

void UThirdPersonMovementComponent::ProcessInput()
{
	auto const hasForwardInput = FMath::Abs(_actorForwardMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;
	auto const hasRightInput = FMath::Abs(_actorRightMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;
	
	if (!hasForwardInput && !hasRightInput)
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
	
	FVector direction = FVector::ZeroVector;
	if (hasForwardInput)
	{
		direction += _cameraComponent->GetForwardVector() * _actorForwardMovementInputAmount;
	}

	if (hasRightInput)
	{
		direction += _cameraComponent->GetRightVector() * _actorRightMovementInputAmount;
	}

	direction = FVector::VectorPlaneProject(direction, FVector::UpVector);
	_inputMovementDirection = direction.GetSafeNormal();
}

void UThirdPersonMovementComponent::CustomTick(const float deltaTime)
{
	ProcessSpringArmRotation(deltaTime);
	ProcessActorRotation(deltaTime, _inputMovementDirection);
}

void UThirdPersonMovementComponent::Init(AActor* actor)
{
	_cameraComponent = actor->FindComponentByClass<UCameraComponent>();
	_springArmComponent = actor->FindComponentByClass<USpringArmComponent>();
}

void UThirdPersonMovementComponent::AddActorRightMovementInput(const float amount)
{
	_actorRightMovementInputAmount += amount;
}

void UThirdPersonMovementComponent::AddActorForwardMovementInput(const float amount)
{
	_actorForwardMovementInputAmount += amount;
}

void UThirdPersonMovementComponent::AddSpringArmYawInput(const float amount)
{
	_springArmYawInput += amount;
}

void UThirdPersonMovementComponent::AddSpringArmPitchInput(const float amount)
{
	_springArmPitchInput += amount;
}

void UThirdPersonMovementComponent::ProcessSpringArmRotation(const float deltaTime) const
{
	auto const hasYawInput = FMath::Abs(_springArmYawInput) > MIN_ROTATION_INPUT;
	auto const hasPitchInput = FMath::Abs(_springArmPitchInput) > MIN_ROTATION_INPUT;
	
	if (!hasYawInput && !hasPitchInput)
	{
		return;
	}

	auto rotation = _springArmComponent->GetRelativeRotation();
	
	if(hasYawInput)
	{
		rotation.Yaw += _springArmYawInput * _springArmYawSpeed * deltaTime;
	}

	if(hasPitchInput)
	{
		rotation.Pitch = ClampSpringArmPitch(rotation.Pitch + _springArmPitchInput * _springArmPitchSpeed * deltaTime);
	}
	
	_springArmComponent->SetRelativeRotation(rotation);
}

float UThirdPersonMovementComponent::ClampSpringArmPitch(float pitch) const
{
	if (pitch > 0)
	{
		if (pitch > _springArmPitchMax)
		{
			pitch = _springArmPitchMax;
		}
	}
	else
	{
		if (pitch < _springArmPitchMin)
		{
			pitch = _springArmPitchMin;
		}
	}
	return pitch;
}

void UThirdPersonMovementComponent::ResetInput()
{
	_actorForwardMovementInputAmount = 0;
	_actorRightMovementInputAmount = 0;
	_springArmYawInput = 0;
	_springArmPitchInput = 0;
}
