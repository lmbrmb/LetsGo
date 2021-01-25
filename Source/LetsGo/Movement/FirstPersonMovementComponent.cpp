#include "FirstPersonMovementComponent.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/Logs/DevLogger.h"

const float MIN_MOVEMENT_INPUT_AMOUNT = 0.1f;
const float MIN_ROTATION_INPUT_AMOUNT = 0.05f;
const float MIN_DOT_FORWARD = -0.01f;

UFirstPersonMovementComponent::UFirstPersonMovementComponent()
{
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
	
	FVector movementDelta;
	bool isMoved;
	ProcessActorMovementTick(DeltaTime, movementDelta, isMoved);
	
	float yawDelta;
	bool isRotated;
	ProcessActorYawTick(DeltaTime, yawDelta, isRotated);
	
	if(isMoved || isRotated)
	{
		auto const rotation = _root->GetComponentRotation();
		auto const newRotation = rotation + FRotator(0, yawDelta, 0);
		_root->MoveComponent(movementDelta, newRotation, true, nullptr, MOVECOMP_NoFlags, ETeleportType::None);
	}

	ResetInput();
}

void UFirstPersonMovementComponent::ProcessActorMovementTick(
	float deltaTime,
	OUT FVector& movementDelta,
	OUT bool& isMoved
) const
{
	auto const hasForwardMovementInput = FMath::Abs(_actorForwardMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;
	auto const hasRightMovementInput = FMath::Abs(_actorRightMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;

	if (!hasForwardMovementInput && !hasRightMovementInput)
	{
		movementDelta = FVector::ZeroVector;
		isMoved = false;
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

	direction = FVector::VectorPlaneProject(direction, FVector::UpVector);
	direction.Normalize();
	
	auto const dotForward = FVector::DotProduct(direction, forwardVector);
	auto const speed = dotForward >= MIN_DOT_FORWARD ? _actorMoveForwardSpeed : _actorMoveBackwardSpeed;
	movementDelta = direction * speed * deltaTime;
	isMoved = true;
}

void UFirstPersonMovementComponent::ProcessActorYawTick(
	float deltaTime,
	float& yawDelta,
	bool& isRotated
) const
{
	auto const hasYawInput = FMath::Abs(_actorYawInputAmount) > MIN_ROTATION_INPUT_AMOUNT;

	if (!hasYawInput)
	{
		yawDelta = 0;
		isRotated = false;
		return;
	}
	
	yawDelta = _actorYawInputAmount * _actorYawSpeed * deltaTime;
	isRotated = true;
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

void UFirstPersonMovementComponent::MapPlayerInput(UInputComponent* playerInputComponent)
{
	_playerInputComponent = playerInputComponent;
	_playerInputComponent->BindAxis(InputConstant::AxisMoveHorizontal, this, &UFirstPersonMovementComponent::AddActorRightMovementInput);
	_playerInputComponent->BindAxis(InputConstant::AxisMoveVertical, this, &UFirstPersonMovementComponent::AddActorForwardMovementInput);
	_playerInputComponent->BindAxis(InputConstant::AxisLookHorizontal, this, &UFirstPersonMovementComponent::AddActorYawInput);
	_playerInputComponent->BindAxis(InputConstant::AxisLookVertical, this, &UFirstPersonMovementComponent::AddCameraPitchInput);
	_playerInputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed, this, &UFirstPersonMovementComponent::Jump);
}

void UFirstPersonMovementComponent::AddActorForwardMovementInput(float amount)
{
	_actorForwardMovementInputAmount = amount;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFirstPersonMovementComponent::AddActorRightMovementInput(float amount)
{
	_actorRightMovementInputAmount = amount;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFirstPersonMovementComponent::AddActorYawInput(float amount)
{
	_actorYawInputAmount = amount;
}

// ReSharper disable once CppMemberFunctionMayBeConst
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
	DevLogger::GetLoggingChannel()->Log("Jump");
}