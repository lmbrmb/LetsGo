#include "MovementComponentBase.h"

#include "LetsGo/Utils/FVectorUtils.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/GameModes/MatchGameMode.h"

const FName UMovementComponentBase::JUMP_FORCE_ID = "Jump";

MovementSpeedState UMovementComponentBase::_defaultMovementSpeedState = MovementSpeedState::Run;

UMovementComponentBase::UMovementComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMovementComponentBase::BeginPlay()
{
	Super::BeginPlay();

	if(_jumpCount > 1 && !_canJumpWhenInAir)
	{
		DevLogger::GetLoggingChannel()->Log("Jump count is more than one but can't jump while in air", LogSeverity::Warning);
	}
	
	auto const owner = GetOwner();
	auto const rootComponent = owner->GetRootComponent();
	RootCollider = Cast<UShapeComponent>(rootComponent);
	AssertIsNotNull(RootCollider);
	CollisionShape = RootCollider->GetCollisionShape();
	CollisionQueryParams.bIgnoreTouches = true;
	CollisionQueryParams.AddIgnoredActor(owner);

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const projectGameModeBase = Cast<AProjectGameModeBase>(authGameMode);
	AssertIsNotNull(projectGameModeBase);
	auto const diContainer = projectGameModeBase->GetDiContainer();

	_rigidBodyComponent = owner->FindComponentByClass<URigidBodyComponent>();
	if(_rigidBodyComponent)
	{
		_rigidBodyComponent->Land.AddUObject(this, &UMovementComponentBase::OnLand);
	}

	Init(owner);

	if(_stepInterval > 0.05f)
	{
		GetWorld()->GetTimerManager().SetTimer(_stepTimerHandle, this, &UMovementComponentBase::StepOnTimer, _stepInterval, true);
	}
}

void UMovementComponentBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (_stepTimerHandle.IsValid())
	{
		auto const world = GetWorld();
		if (world)
		{
			world->GetTimerManager().ClearTimer(_stepTimerHandle);
		}
	}
}

void UMovementComponentBase::ProcessActorRotation(const float deltaTime, const FVector& direction) const
{
	auto const actorForwardDirection = RootCollider->GetForwardVector();
	auto const targetDirectionDot = FVector::DotProduct(actorForwardDirection, direction);

	if (targetDirectionDot >= SKIP_ROTATION_DOT)
	{
		return;
	}

	auto const targetAngleDegrees = FVectorUtils::GetUnsignedAngleDegrees(targetDirectionDot);
	auto const targetAngleSign = FVectorUtils::GetSignOfAngle(actorForwardDirection, direction);
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

void UMovementComponentBase::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessInput();
	CheckGround();
	ProcessMovement(DeltaTime);
	CustomTick(DeltaTime);
	ResetInput();
}

void UMovementComponentBase::CheckGround()
{
	auto const location = RootCollider->GetComponentLocation();
	auto const targetLocation = location + FVector::DownVector * 10;
	auto const isOnGround = GetWorld()->SweepSingleByChannel(
		_groundHitResult,
		location,
		targetLocation,
		RootCollider->GetComponentQuat(),
		_collisionChannel,
		CollisionShape,
		CollisionQueryParams
	);
}

void UMovementComponentBase::OnLand(const float airTime)
{
	_jumpIndex = 0;
}

inline void UMovementComponentBase::ProcessMovement(const float deltaTime)
{
	auto const direction = GetMovementDirection();
	if (direction.IsNearlyZero())
	{
		return;
	}

	auto const baseMovementSpeed = GetBaseMovementSpeed();
	if (FMath::IsNearlyZero(baseMovementSpeed))
	{
		return;
	}

	auto const environmentMultiplier = GetEnvironmentSpeedMultiplier();
	auto const speedStateMultiplier = GetIsInAir() ? 1.0f : GetSpeedStateMultiplier();
	auto const speed = baseMovementSpeed * environmentMultiplier * speedStateMultiplier;
	auto const translationAmount = speed * deltaTime;
	auto const rootColliderLocation = RootCollider->GetComponentLocation();
	auto const rootColliderRotation = RootCollider->GetComponentQuat();
	Move(
		rootColliderLocation,
		rootColliderRotation,
		direction,
		_groundHitResult,
		translationAmount
	);
}

float UMovementComponentBase::GetEnvironmentSpeedMultiplier() const
{
	auto const environmentMultiplier = GetIsInAir() ? _airMultiplier : 1.0f;
	return environmentMultiplier;
}

float UMovementComponentBase::GetSpeedStateMultiplier() const
{
	auto const speedStateMultiplier =
		_movementSpeedState == MovementSpeedState::Sprint ? _sprintMultiplier :
		_movementSpeedState == MovementSpeedState::Walk ? _walkMultiplier :
		1.0f;
	return speedStateMultiplier;
}

void UMovementComponentBase::PerformJump()
{
	// RigidBodyComponent is must have, jump implementation depends on it
	AssertIsNotNull(_rigidBodyComponent);

	if(!CanJump())
	{
		return;
	}

	_jumpIndex++;

	Jump.Broadcast();

	// Current jump force will be replaced with new one
	_rigidBodyComponent->RemoveForce(JUMP_FORCE_ID);

	// Jump up force
	auto const jumpUpDirection = FVector::UpVector;
	_rigidBodyComponent->AddForce(
		JUMP_FORCE_ID,
		jumpUpDirection,
		_jumpForceUpCurve,
		_jumpForceCurveMagnitudeMultiplier,
		_jumpForceCurveTimeMultiplier
	);

	// Jump velocity force
	auto const velocity = FVector::VectorPlaneProject(_rigidBodyComponent->GetVelocity(), FVector::UpVector);
	if(FMath::IsNearlyZero(velocity.SizeSquared(), 0.1f))
	{
		return;
	}
	
	auto const jumpVelocityDirection = velocity.GetSafeNormal();
	_rigidBodyComponent->AddForce(
		JUMP_FORCE_ID,
		jumpVelocityDirection,
		_jumpForceVelocityCurve,
		_jumpForceCurveMagnitudeMultiplier,
		_jumpForceCurveTimeMultiplier
	);
}

bool UMovementComponentBase::CanJump() const
{
	if (!_rigidBodyComponent)
	{
		return false;
	}

	if (!_canJumpWhenInAir && GetIsInAir())
	{
		return false;
	}

	if (_jumpIndex >= _jumpCount)
	{
		return false;
	}

	return true;
}

FVector UMovementComponentBase::GetRootColliderLocation() const
{
	return RootCollider->GetComponentLocation();
}

void UMovementComponentBase::ActivateMovementSpeedState(MovementSpeedState movementSpeedState)
{
	_movementSpeedState = movementSpeedState;
}

void UMovementComponentBase::DeactivateMovementSpeedState(MovementSpeedState movementSpeedState)
{
	if(_movementSpeedState != movementSpeedState)
	{
		return;
	}

	_movementSpeedState = _defaultMovementSpeedState;
}

bool UMovementComponentBase::GetIsInAir() const
{
	if(!_rigidBodyComponent)
	{
		return false;
	}

	return _rigidBodyComponent->GetIsInAir();
}

void UMovementComponentBase::Move(
	const FVector& rootColliderLocation,
	const FQuat& rootColliderRotation,
	const FVector& inputDirection,
	const FHitResult& groundHitResult,
	const float translationAmount
)
{
	FVector translation;
	if(!groundHitResult.bBlockingHit)
	{
		// No block - potentially in air
		translation = inputDirection * translationAmount;
		RootCollider->AddWorldOffset(translation, true);
		return;
	}

	//Sweep in direction
	auto const groundHitNormal = groundHitResult.Normal;
	auto projectedDirection = FVector::VectorPlaneProject(inputDirection, groundHitNormal).GetSafeNormal();
	translation = projectedDirection * translationAmount;
	auto const targetEndLocation = rootColliderLocation + translation;
	
	auto const isBlocked = GetWorld()->SweepSingleByChannel(
		_bufferHitResult,
		rootColliderLocation,
		targetEndLocation,
		rootColliderRotation,
		_collisionChannel,
		CollisionShape,
		CollisionQueryParams
	);
	
	if (!isBlocked)
	{
		auto const groundDirectionDot = FVector::DotProduct(inputDirection, groundHitNormal);
		if(groundDirectionDot < 0.1f)
		{
			// Ground (possibly slight slope)
			// No need to sweep because movement end position is already checked
			RootCollider->SetWorldLocation(targetEndLocation, false);
		}
		else
		{
			// Touched wall with back side of root collider while falling down (ground->fall transition)
			translation = inputDirection * translationAmount;
			RootCollider->AddWorldOffset(translation, true);
		}
		
		return;
	}

	// Moving along plane normal
	// Ladders, movement along wall
	auto const planeNormal = _bufferHitResult.Normal;
	auto const inputDirectionDot = FVector::DotProduct(planeNormal, inputDirection);
	auto const upDot = FVector::DotProduct(FVector::UpVector, planeNormal);
	auto const isWall = FMath::Abs(upDot) < 0.1f;
	projectedDirection = FVector::VectorPlaneProject(inputDirection, planeNormal).GetSafeNormal();
	auto const directionCoefficient = isWall ? 1 - FMath::Abs(inputDirectionDot) : 1;
	translation = projectedDirection * directionCoefficient * translationAmount;
	RootCollider->AddWorldOffset(translation, true);
}

// Template methods below

void UMovementComponentBase::Init(AActor* actor)
{
	AssertDefaultImplementationIsOverriden();
}

float UMovementComponentBase::GetBaseMovementSpeed()
{
	AssertDefaultImplementationIsOverriden(0);
}

void UMovementComponentBase::CustomTick(const float deltaTime)
{
	AssertDefaultImplementationIsOverriden();
}

void UMovementComponentBase::ProcessInput()
{
	AssertDefaultImplementationIsOverriden();
}

float UMovementComponentBase::GetAbsoluteMovementAmount() const
{
	AssertDefaultImplementationIsOverriden(0);
}

const FVector& UMovementComponentBase::GetMovementDirection() const
{
	AssertDefaultImplementationIsOverriden(FVector::ZeroVector);
}

void UMovementComponentBase::ResetInput()
{
	AssertDefaultImplementationIsOverriden();
}

void UMovementComponentBase::StepOnTimer() const
{
	if(GetIsInAir() || GetAbsoluteMovementAmount() <= 0)
	{
		return;
	}

	Step.Broadcast(_movementSpeedState);
}
