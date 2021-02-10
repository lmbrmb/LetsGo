#include "FirstPersonMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "LetsGo/Logs/DevLogger.h"

const float MIN_MOVEMENT_INPUT_AMOUNT = 0.1f;
const float MIN_ROTATION_INPUT_AMOUNT = 0.05f;
const float MIN_DOT_FORWARD = -0.01f;

UFirstPersonMovementComponent::UFirstPersonMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UFirstPersonMovementComponent::~UFirstPersonMovementComponent()
{
}

void UFirstPersonMovementComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessCameraPitchTick(DeltaTime);
	ProcessActorYawTick(DeltaTime);
	ProcessActorMovementTick(DeltaTime);
	ResetInput();
}

void UFirstPersonMovementComponent::ProcessActorMovementTick(float deltaTime)
{
	auto const hasForwardMovementInput = FMath::Abs(_actorForwardMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;
	auto const hasRightMovementInput = FMath::Abs(_actorRightMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;

	if (!hasForwardMovementInput && !hasRightMovementInput)
	{
		_movementAmount = 0;
		return;
	}

	FVector direction;

	auto const forwardVector = _root->GetForwardVector();
	
	if (hasForwardMovementInput)
	{
		direction += forwardVector * _actorForwardMovementInputAmount;
	}
	
	if (hasRightMovementInput)
	{
		direction += _root->GetRightVector() * _actorRightMovementInputAmount;
	}

	auto absoluteMovementAmount = FMath::Abs(_actorForwardMovementInputAmount) + FMath::Abs(_actorRightMovementInputAmount);
	if(absoluteMovementAmount > 1)
	{
		absoluteMovementAmount = 1;
	}
	_movementAmount = absoluteMovementAmount;
	
	direction = FVector::VectorPlaneProject(direction, FVector::UpVector);
	direction.Normalize();

	auto const actorLocation = _root->GetComponentLocation();
	DrawDebugLine(GetWorld(), actorLocation, actorLocation + direction * 150, FColor::Blue);
	
	auto const dotForward = FVector::DotProduct(direction, forwardVector);
	auto const isMovingForward = dotForward >= MIN_DOT_FORWARD;
	auto const speed = isMovingForward
		? _actorMoveForwardSpeed * (_isSprinting ? _sprintMultiplier : 1.0f)
		: _actorMoveBackwardSpeed;
	auto const movementDelta = direction * speed * absoluteMovementAmount * deltaTime;
	_root->AddRelativeLocation(movementDelta);
}

void UFirstPersonMovementComponent::ProcessActorYawTick(float deltaTime)
{
	auto const hasYawInput = FMath::Abs(_actorYawInputAmount) > MIN_ROTATION_INPUT_AMOUNT;

	if (!hasYawInput)
	{
		return;
	}
	
	auto const yawDelta = _actorYawInputAmount * _actorYawSpeed * deltaTime;
	_root->AddRelativeRotation(FRotator(0, yawDelta, 0));
}

void UFirstPersonMovementComponent::ProcessCameraPitchTick(float deltaTime) const
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

void UFirstPersonMovementComponent::Init(AActor* actor)
{
	_root = actor->GetRootComponent();
	_cameraComponent = actor->FindComponentByClass<UCameraComponent>();
}

float UFirstPersonMovementComponent::GetMovementAmount()
{
	return _movementAmount;
}

void UFirstPersonMovementComponent::AddActorForwardMovementInput(const float amount)
{
	_actorForwardMovementInputAmount = amount;
}

void UFirstPersonMovementComponent::AddActorRightMovementInput(const float amount)
{
	_actorRightMovementInputAmount = amount;
}

void UFirstPersonMovementComponent::AddActorYawInput(const float amount)
{
	_actorYawInputAmount = amount;
}

void UFirstPersonMovementComponent::AddCameraPitchInput(float amount)
{
	_cameraPitchInputAmount = amount;
}

inline void UFirstPersonMovementComponent::ResetInput()
{
	_actorForwardMovementInputAmount = 0;
	_actorRightMovementInputAmount = 0;
	_cameraPitchInputAmount = 0;
	_actorYawInputAmount = 0;
}

void UFirstPersonMovementComponent::Jump()
{
	// TODO: implement
	DevLogger::GetLoggingChannel()->Log("Jump");
}

void UFirstPersonMovementComponent::StartSprint()
{
	_isSprinting = true;
}

void UFirstPersonMovementComponent::StopSprint()
{
	_isSprinting = false;
}
