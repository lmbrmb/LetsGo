#include "FirstPersonMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "LetsGo/Logs/DevLogger.h"

const float MIN_MOVEMENT_INPUT_AMOUNT = 0.1f;
const float MIN_ROTATION_INPUT_AMOUNT = 0.05f;
const float MIN_DOT_FORWARD = -0.01f;
const bool DETECT_COLLISION_ON_MOVEMENT = true;
const FName JUMP_FORCE_NAME = FName("Jump");
const FName GRAVITY_FORCE_NAME = FName("Gravity");

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

	CheckGround();
	ProcessForcesTick(DeltaTime);
	ProcessCameraPitchTick(DeltaTime);
	ProcessActorYawTick(DeltaTime);
	auto const targetMovementAmount = GetInputMovementAmount(DeltaTime);
	AddMovement(targetMovementAmount);

	auto const location = _root->GetComponentLocation();
	_velocity = location - _previousLocation;
	_previousLocation = location;
	ResetInput();
}

FVector UFirstPersonMovementComponent::GetInputMovementAmount(float deltaTime)
{
	auto const hasForwardMovementInput = FMath::Abs(_actorForwardMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;
	auto const hasRightMovementInput = FMath::Abs(_actorRightMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;

	if (!hasForwardMovementInput && !hasRightMovementInput)
	{
		_movementAmount = 0;
		return FVector::ZeroVector;
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
	DrawDebugLine(_world, actorLocation, actorLocation + direction * 150, FColor::Blue);
	
	auto const dotForward = FVector::DotProduct(direction, forwardVector);
	auto const isMovingForward = dotForward >= MIN_DOT_FORWARD;
	auto const directionSpeed = isMovingForward
		? _actorMoveForwardSpeed * (_isSprinting ? _sprintMultiplier : 1.0f)
		: _actorMoveBackwardSpeed;
	auto const environmentMultiplier = _isInAir ? 0.25f : 1.0f;
	auto const movementDelta = direction * directionSpeed * absoluteMovementAmount * environmentMultiplier * deltaTime;
	return movementDelta;
}

void UFirstPersonMovementComponent::CheckGround()
{
	auto const location = _root->GetComponentLocation();
	auto const targetLocation = location + FVector::DownVector * 10;
	
	auto const isOnGround = _world->SweepSingleByChannel(
		_floorHitResult,
		location,
		targetLocation,
		_root->GetComponentQuat(),
		ECC_WorldStatic,
		_collisionShape,
		_collisionQueryParams
	);
	
	_isInAir = !isOnGround;
}

void UFirstPersonMovementComponent::AddMovement(FVector inputMovementAmount)
{
	_root->AddRelativeLocation(inputMovementAmount, DETECT_COLLISION_ON_MOVEMENT);
}

void UFirstPersonMovementComponent::ProcessForcesTick(float deltaTime)
{
	auto const forcesCount = _forces.Num();

	if(forcesCount <= 0)
	{
		return;
	}
	
	auto forceSum = FVector::ZeroVector;

	for (auto i = forcesCount - 1; i>=0 ; i--)
	{
		auto const force = _forces[i];
		
		if(force->IsActive())
		{
			const auto forceVector = force->Take(deltaTime);
			forceSum += forceVector;
		}
		else
		{
			_forces.RemoveAt(i);
		}
	}
	_root->AddRelativeLocation(forceSum, DETECT_COLLISION_ON_MOVEMENT);
}

void UFirstPersonMovementComponent::ProcessActorYawTick(float deltaTime) const
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
	_world = GetWorld();
	_root = actor->GetRootComponent();
	_rootCollider = actor->FindComponentByClass<UShapeComponent>();
	_collisionShape = _rootCollider->GetCollisionShape();
	_cameraComponent = actor->FindComponentByClass<UCameraComponent>();
	_collisionQueryParams.AddIgnoredActor(actor);
	
	if(!FMath::IsNearlyZero(_gravityForceMagnitude) )
	{
		auto const gravityForce = Force::CreateInfiniteForce(GRAVITY_FORCE_NAME, FVector::DownVector, _gravityForceMagnitude);
		_forces.Add(gravityForce);
	}
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

void UFirstPersonMovementComponent::ResetInput()
{
	_actorForwardMovementInputAmount = 0;
	_actorRightMovementInputAmount = 0;
	_cameraPitchInputAmount = 0;
	_actorYawInputAmount = 0;
}

void UFirstPersonMovementComponent::Jump()
{
	if (!_isInAir)
	{
		_jumpIndex = 0;
	}

	if(_jumpIndex >= _jumpCount)
	{
		return;
	}

	_jumpIndex++;

	for (auto i = _forces.Num() - 1; i >= 0; i--)
	{
		auto const force = _forces[i];
		if (force->GetId() == JUMP_FORCE_NAME)
		{
			_forces.RemoveAt(i);
		}
	}

	auto const normalizedVelocity = _velocity.GetSafeNormal();
	auto jumpDirection = FVector::UpVector + normalizedVelocity;
	jumpDirection.Normalize();

	auto const dotUp = FVector::DotProduct(jumpDirection, FVector::UpVector);
	auto const multiplier = 2 - dotUp;
	auto const magnitude = _jumpForceMagnitude * multiplier;
	auto const jumpForce = Force::CreateFiniteForce(JUMP_FORCE_NAME, _jumpForceDuration, jumpDirection, magnitude);
	_forces.Add(jumpForce);
}

void UFirstPersonMovementComponent::StartSprint()
{
	_isSprinting = true;
}

void UFirstPersonMovementComponent::StopSprint()
{
	_isSprinting = false;
}
