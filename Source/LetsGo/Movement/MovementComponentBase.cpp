#include "MovementComponentBase.h"
#include "DrawDebugHelpers.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/FVectorUtils.h"

UMovementComponentBase::UMovementComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UMovementComponentBase::~UMovementComponentBase()
{
	//Do nothing
}

void UMovementComponentBase::BeginPlay()
{
	Super::BeginPlay();
	
	auto const actor = GetOwner();
	World = GetWorld();
	Root = actor->GetRootComponent();
	RootCollider = actor->FindComponentByClass<UShapeComponent>();
	CollisionShape = RootCollider->GetCollisionShape();
	CollisionQueryParams.AddIgnoredActor(actor);
	
	if (!FMath::IsNearlyZero(_gravityForceMagnitude))
	{
		auto const gravityForce = Force::CreateInfiniteForce(GRAVITY_FORCE_NAME, FVector::DownVector, _gravityForceMagnitude);
		_forces.Add(gravityForce);
	}
	
	Init(actor);
}

void UMovementComponentBase::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckGround();
	ProcessInput();
	ProcessForces(DeltaTime);
	ProcessMovement(DeltaTime);
	CustomTick(DeltaTime);
	UpdateVelocity();
	ResetInput();
}

void UMovementComponentBase::CheckGround()
{
	auto const location = Root->GetComponentLocation();
	auto const targetLocation = location + FVector::DownVector * 10;
	auto const isOnGround = World->SweepSingleByChannel(
		_groundHitResult,
		location,
		targetLocation,
		Root->GetComponentQuat(),
		ECC_WorldStatic,
		CollisionShape,
		CollisionQueryParams
	);

	IsInAir = !isOnGround;
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

		if (force->IsActive())
		{
			const auto forceVector = force->Take(deltaTime);
			forceSum += forceVector;
		}
		else
		{
			_forces.RemoveAt(i);
		}
	}
	Root->AddRelativeLocation(forceSum, true);
}

inline void UMovementComponentBase::ProcessMovement(const float& deltaTime)
{
	auto const direction = GetInputMovementDirection();
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
	auto const currentLocation = Root->GetComponentLocation();
	auto const currentRotation = Root->GetComponentQuat();
	Move(
		currentLocation,
		currentRotation,
		direction,
		_groundHitResult,
		translationAmount,
		FIRST_MOVE_CALL_NUMBER
	);
	//TODO: slide down surface
}

void UMovementComponentBase::Jump()
{
	if (!IsInAir)
	{
		_jumpIndex = 0;
	}

	if (_jumpIndex >= _jumpCount)
	{
		return;
	}

	_jumpIndex++;

	for (auto i = _forces.Num() - 1; i >= 0; i--)
	{
		auto const force = _forces[i];
		if (force->GetId() == JUMP_FORCE_ID)
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
	auto const jumpForce = Force::CreateFiniteForce(JUMP_FORCE_ID, _jumpForceDuration, jumpDirection, magnitude);

	// Replacing current jump force if exists
	_forces.RemoveAll([](Force* f) {return f->GetId() == JUMP_FORCE_ID; });
	_forces.Add(jumpForce);
}

void UMovementComponentBase::Move(
	const FVector& rootLocation,
	const FQuat& rootRotation,
	const FVector& direction,
	const FHitResult& planeHitResult,
	const float translationAmount,
	const int callNumber
)
{
	if (!planeHitResult.bBlockingHit)
	{
		// Not blocked
		auto const translation = direction * translationAmount;
		Root->AddWorldOffset(translation, true);
		return;
	}

	auto const planeNormal = planeHitResult.Normal;
	auto const angleDegrees = FVectorUtils::GetUnsignedAngleDegrees(direction, planeNormal);
	
	if (angleDegrees > _maxSlopeDegreesUp)
	{
		// Slope is too steep, potentially a wall - can't move
		//TODO: move along wall
		return;
	}

	if (angleDegrees < _maxSlopeDegreesDown)
	{
		// Slope break - no translation restriction
		auto const translation = direction * translationAmount;
		Root->AddWorldOffset(translation, true);
		return;
	}

	auto const directionProjectedOnPlane = FVector::VectorPlaneProject(direction, planeNormal);
	auto const translation = directionProjectedOnPlane * translationAmount;
	auto const castLocation = rootLocation + translation;
	
	auto const isBlocked = World->SweepSingleByChannel(
		_bufferHitResult,
		rootLocation,
		castLocation,
		rootRotation,
		ECC_WorldStatic,
		CollisionShape,
		CollisionQueryParams
	);

	if (!isBlocked)
	{
		Root->AddWorldOffset(translation, false);
		return;
	}
	
	if (callNumber >= MAX_MOVE_CALL_DEPTH)
	{
		DevLogger::GetLoggingChannel()->LogValue("Move() reached max call depth", MAX_MOVE_CALL_DEPTH, LogSeverity::Warning);
		return;
	}
	
	Move(
		rootLocation,
		rootRotation,
		direction,
		_bufferHitResult,
		translationAmount,
		callNumber + 1
	);
}

void UMovementComponentBase::UpdateVelocity()
{
	auto const location = Root->GetComponentLocation();
	_velocity = location - _previousLocation;
	_previousLocation = location;
}

// Template methods below

void UMovementComponentBase::Init(AActor* actor)
{
	//Do nothing
}

float UMovementComponentBase::GetMovementSpeed()
{
	//Stub
	return 0;
}

void UMovementComponentBase::CustomTick(float deltaTime)
{
	//Do nothing
}

void UMovementComponentBase::ProcessInput()
{
	//Do nothing
}

float UMovementComponentBase::GetAbsoluteMovementAmount()
{
	//Stub
	return 0;
}

FVector UMovementComponentBase::GetInputMovementDirection()
{
	//Stub
	return FVector::ZeroVector;
}

void UMovementComponentBase::ResetInput()
{
	//Do nothing
}
