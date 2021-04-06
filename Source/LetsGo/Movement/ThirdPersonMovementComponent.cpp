#include "ThirdPersonMovementComponent.h"
#include "LetsGo/Utils/FVectorUtils.h"

const float MIN_MOVEMENT_INPUT_AMOUNT = 0.15f;
const float MIN_ROTATION_INPUT = 0.05f;
const float MIN_MOVEMENT_DOT = 0.25f;
const float SKIP_ROTATION_DOT = 0.99f;

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
	const auto actorForwardDirection = RootCollider->GetForwardVector();
	const auto targetDirectionDot = FVector::DotProduct(actorForwardDirection, _inputMovementDirection);
	
	if (targetDirectionDot >= SKIP_ROTATION_DOT)
	{
		return;
	}

	auto const targetAngleDegrees = FVectorUtils::GetUnsignedAngleDegrees(targetDirectionDot);
	auto const targetAngleSign = FVectorUtils::GetSignOfAngle(actorForwardDirection, _inputMovementDirection);
	auto rotationDeltaDegrees = deltaTime * _rotationSpeedDegrees;

	if (rotationDeltaDegrees > targetAngleDegrees)
	{
		rotationDeltaDegrees = targetAngleDegrees;
	}

	auto const rotationSignedAngleDegrees = targetAngleSign * rotationDeltaDegrees;
	auto const rotationVector = actorForwardDirection.RotateAngleAxis(rotationSignedAngleDegrees, FVector::UpVector);
	auto const actorRotation = FRotationMatrix::MakeFromX(rotationVector).Rotator();
	
	//auto const actorLocation = Root->GetComponentLocation();
	//DrawDebugLine(GetWorld(), actorLocation, actorLocation + rotationVector * 100, FColor::Yellow);
	
	RootCollider->SetWorldRotation(actorRotation);
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
