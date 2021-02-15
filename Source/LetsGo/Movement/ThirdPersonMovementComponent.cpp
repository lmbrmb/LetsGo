#include "ThirdPersonMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

const float MIN_MOVEMENT_INPUT_AMOUNT = 0.15f;
const float MIN_ROTATION_INPUT = 0.05f;
const float MIN_MOVEMENT_DOT = 0.25f;
const float SKIP_ROTATION_DOT = 0.99f;

float UThirdPersonMovementComponent::GetAbsoluteMovementAmount()
{
	return _absoluteMovementAmount;
}

FVector UThirdPersonMovementComponent::GetInputMovementDirection()
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
	direction.Normalize();
	_inputMovementDirection = direction;

	/*auto const actorLocation = Root->GetComponentLocation();
	DrawDebugLine(GetWorld(), actorLocation, actorLocation + _inputMovementDirection * 100, FColor::Green);*/
}

void UThirdPersonMovementComponent::CustomTick(float deltaTime)
{
	ProcessSpringArmRotation(deltaTime);
	ProcessActorRotation(deltaTime);
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

void UThirdPersonMovementComponent::ProcessActorRotation(const float deltaTime) const
{
	const auto actorForwardDirection = Root->GetForwardVector();
	const auto targetDirectionDot = FVector::DotProduct(actorForwardDirection, _inputMovementDirection);
	
	if (targetDirectionDot >= SKIP_ROTATION_DOT)
	{
		return;
	}
	auto const targetAngleRadians = FMath::Acos(targetDirectionDot);
	auto const targetAngleDegrees = FMath::RadiansToDegrees(targetAngleRadians);
	const auto targetDirectionCross = FVector::CrossProduct(actorForwardDirection, _inputMovementDirection);
	auto const targetAngleSign = FMath::Sign(FVector::DotProduct(FVector::UpVector, targetDirectionCross));
	auto rotationDeltaDegrees = deltaTime * _rotationSpeedDegrees;

	if (rotationDeltaDegrees > targetAngleDegrees)
	{
		rotationDeltaDegrees = targetAngleDegrees;
	}

	auto const rotationSignedAngleDegrees = targetAngleSign * rotationDeltaDegrees;
	auto const rotationVector = actorForwardDirection.RotateAngleAxis(rotationSignedAngleDegrees, FVector::UpVector);

	//auto const actorLocation = Root->GetComponentLocation();
	//DrawDebugLine(GetWorld(), actorLocation, actorLocation + rotationVector * 100, FColor::Yellow);
	
	auto const actorRotation = UKismetMathLibrary::MakeRotFromX(rotationVector);
	Root->SetWorldRotation(actorRotation);
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
