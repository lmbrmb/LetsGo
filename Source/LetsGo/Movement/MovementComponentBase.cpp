#include "MovementComponentBase.h"

#include "LetsGo/Utils/FVectorUtils.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "DrawDebugHelpers.h"
#include "LetsGo/GameModes/MatchGameMode.h"

const FName UMovementComponentBase:: GRAVITY_FORCE_ID = "Gravity";

const FName UMovementComponentBase::JUMP_FORCE_ID = "Jump";

UMovementComponentBase::UMovementComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMovementComponentBase::BeginPlay()
{
	Super::BeginPlay();
	
	auto const actor = GetOwner();
	auto const rootComponent = actor->GetRootComponent();
	RootCollider = Cast<UShapeComponent>(rootComponent);
	AssertIsNotNull(RootCollider);
	CollisionShape = RootCollider->GetCollisionShape();
	CollisionQueryParams.bIgnoreTouches = true;
	CollisionQueryParams.AddIgnoredActor(actor);
	
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const projectGameModeBase = Cast<AProjectGameModeBase>(authGameMode);
	AssertIsNotNull(projectGameModeBase);
	auto const diContainer = projectGameModeBase->GetDiContainer();

	auto const forceFactory = diContainer->GetInstance<ForceFactory>();
	_forceFactory = &forceFactory.Get();
	
	if (!FMath::IsNearlyZero(_gravityForceMagnitude))
	{
		auto const gravityForce = _forceFactory->Create(GRAVITY_FORCE_ID, FVector::DownVector, _gravityForceMagnitude);
		_forces.Add(gravityForce);
	}
	
	Init(actor);

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
	const auto actorForwardDirection = RootCollider->GetForwardVector();
	const auto targetDirectionDot = FVector::DotProduct(actorForwardDirection, direction);

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
	ProcessForces(DeltaTime);
	CheckGround();
	ProcessMovement(DeltaTime);
	CustomTick(DeltaTime);
	UpdateVelocity();
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

	SetIsInAir(!isOnGround);
}

void UMovementComponentBase::ProcessForces(const float& deltaTime)
{
	auto const forcesCount = _forces.Num();

	if (forcesCount <= 0)
	{
		return;
	}

	auto forceSum = FVector::ZeroVector;

	for (auto i = forcesCount - 1; i >= 0; i--)
	{
		auto const force = _forces[i];
		const auto forceVector = force->GetVector(deltaTime);
		forceSum += forceVector;
	}
	auto const deltaLocation = forceSum * deltaTime;
	RootCollider->AddRelativeLocation(deltaLocation, true);
}

inline void UMovementComponentBase::ProcessMovement(const float& deltaTime)
{
	auto const direction = GetMovementDirection();
	if (direction.IsNearlyZero())
	{
		return;
	}
	auto const speed = GetMovementSpeed();

	if (FMath::IsNearlyZero(speed))
	{
		return;
	}

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

void UMovementComponentBase::PerformJump()
{
	if (_jumpIndex >= _jumpCount)
	{
		return;
	}

	_jumpIndex++;

	Jump.Broadcast();
	
	// Current jump force will be replaced with new one
	_forces.RemoveAll([](IForce* f) {return f->GetId() == JUMP_FORCE_ID; });

	// Jump up force
	auto const jumpUpDirection = FVector::UpVector;
	auto const jumpForceUp = _forceFactory->Create(
		JUMP_FORCE_ID,
		jumpUpDirection,
		_jumpForceUpCurve,
		_jumpForceCurveMagnitudeMultiplier,
		_jumpForceCurveTimeMultiplier
	);
	_forces.Add(jumpForceUp);

	// Jump velocity force
	auto const velocity = FVector::VectorPlaneProject(_velocity, FVector::UpVector);
	if(FMath::IsNearlyZero(velocity.SizeSquared(), 0.1f))
	{
		return;
	}
	
	auto const jumpVelocityDirection = velocity.GetSafeNormal();
	auto const jumpVelocityForce = _forceFactory->Create(
		JUMP_FORCE_ID,
		jumpVelocityDirection,
		_jumpForceVelocityCurve,
		_jumpForceCurveMagnitudeMultiplier,
		_jumpForceCurveTimeMultiplier
	);
	_forces.Add(jumpVelocityForce);
}

FVector UMovementComponentBase::GetRootColliderLocation() const
{
	return RootCollider->GetComponentLocation();
}

bool UMovementComponentBase::GetIsInAir() const
{
	return _isInAir;
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
	auto planeNormal = groundHitResult.Normal;
	auto projectedDirection = FVector::VectorPlaneProject(inputDirection, planeNormal).GetSafeNormal();
	translation = projectedDirection * translationAmount;
	auto const castEndLocation = rootColliderLocation + translation;
	
	auto const isBlocked = GetWorld()->SweepSingleByChannel(
		_bufferHitResult,
		rootColliderLocation,
		castEndLocation,
		rootColliderRotation,
		_collisionChannel,
		CollisionShape,
		CollisionQueryParams
	);

	AssertIsEqual(isBlocked, (bool)_bufferHitResult.bBlockingHit);
	
	if (!isBlocked)
	{
		// No block - can move along plane
		// No need to sweep because movement end position is already checked
		RootCollider->AddWorldOffset(translation, false);
		return;
	}

	planeNormal = _bufferHitResult.Normal;
	auto const obstacleDeltaZ = rootColliderLocation.Z - _bufferHitResult.ImpactPoint.Z;
	auto const canStepOn = obstacleDeltaZ < _maxStepHeight;
	if (!canStepOn)
	{
		// The obstacle is too high - can't step on it
		return;
	}

	// Moving along plane normal
	projectedDirection = FVector::VectorPlaneProject(inputDirection, planeNormal).GetSafeNormal();
	translation = projectedDirection * translationAmount;
	RootCollider->AddWorldOffset(translation, true);
}

void UMovementComponentBase::UpdateVelocity()
{
	auto const location = RootCollider->GetComponentLocation();
	_velocity = location - _previousLocation;
	_previousLocation = location;
}

void UMovementComponentBase::SetIsInAir(const bool isInAir)
{
	auto const wasInAir = _isInAir;

	_isInAir = isInAir;

	if (wasInAir != _isInAir)
	{
		if (!isInAir)
		{
			_jumpIndex = 0;
			_forces.RemoveAll([](IForce* f) {return f->GetId() == JUMP_FORCE_ID; });
			Land.Broadcast();
		}
	}
}

// Template methods below

void UMovementComponentBase::Init(AActor* actor)
{
	AssertDefaultImplementationIsOverriden();
}

float UMovementComponentBase::GetMovementSpeed()
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
	if(_isInAir || GetAbsoluteMovementAmount() <= 0)
	{
		return;
	}

	Step.Broadcast();
}
