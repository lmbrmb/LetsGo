#include "MovementComponentBase.h"

#include "DrawDebugHelpers.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/FVectorUtils.h"

#include "LetsGo/GameModes/MatchGameMode.h"

const FName UMovementComponentBase:: GRAVITY_FORCE_ID = "Gravity";

const FName UMovementComponentBase::JUMP_FORCE_ID = "Jump";

const float UMovementComponentBase::SLOPE_ZERO = 90.0f;

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
	
	auto const authGameMode = World->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();

	auto const forceFactory = diContainer->GetInstance<ForceFactory>();
	_forceFactory = &forceFactory.Get();
	
	if (!FMath::IsNearlyZero(_gravityForceMagnitude))
	{
		auto const gravityForce = _forceFactory->Create(GRAVITY_FORCE_ID, FVector::DownVector, _gravityForceMagnitude);
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

	auto const wasInAir = _isInAir;
	
	_isInAir = !isOnGround;

	if(wasInAir != _isInAir)
	{
		if(isOnGround)
		{
			_jumpIndex = 0;
			_forces.RemoveAll([](IForce* f) {return f->GetId() == JUMP_FORCE_ID; });
		}
	}
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
	Root->AddRelativeLocation(deltaLocation, true);
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
}

void UMovementComponentBase::Jump()
{
	if (_jumpIndex >= _jumpCount)
	{
		return;
	}

	_jumpIndex++;
	
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

bool UMovementComponentBase::GetIsInAir() const
{
	return _isInAir;
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
		// Not blocked, in air
		auto const translation = direction * translationAmount;
		Root->AddWorldOffset(translation, true);
		return;
	}

	auto const planeNormal = planeHitResult.Normal;
	auto const angleDegrees = FVectorUtils::GetUnsignedAngleDegrees(direction, planeNormal);
	
	if (angleDegrees > _maxSlopeDegreesUp)
	{
		// Slope is too steep, potentially a wall - can't move
		// TODO: try step on it
		return;
	}

	if (angleDegrees < _maxSlopeDegreesDown)
	{
		// Slope break - no translation restriction
		auto const translation = direction * translationAmount;
		Root->AddWorldOffset(translation, true);
		return;
	}

	// Moving along plane
	auto const directionProjectedOnPlane = FVector::VectorPlaneProject(direction, planeNormal);
	auto const translation = directionProjectedOnPlane * translationAmount;
	auto const castEndLocation = rootLocation + translation;
	
	auto const isBlocked = World->SweepSingleByChannel(
		_bufferHitResult,
		rootLocation,
		castEndLocation,
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
