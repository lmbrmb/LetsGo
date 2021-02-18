#include "MovementComponentBase.h"

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

	auto const direction = GetInputMovementDirection();
	if(direction != FVector::ZeroVector)
	{
		auto const speed = GetMovementSpeed();
		auto const translation = direction * speed * DeltaTime;
		Translate(translation);
	}
	
	CustomTick(DeltaTime);
	UpdateVelocity();
	ResetInput();
}

void UMovementComponentBase::CheckGround()
{
	auto const location = Root->GetComponentLocation();
	auto const targetLocation = location + FVector::DownVector * 10;

	auto const isOnGround = World->SweepSingleByChannel(
		_floorHitResult,
		location,
		targetLocation,
		Root->GetComponentQuat(),
		ECC_WorldStatic,
		CollisionShape,
		CollisionQueryParams
	);

	IsInAir = !isOnGround;
}

void UMovementComponentBase::ProcessForces(float deltaTime)
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
	Root->AddRelativeLocation(forceSum, DETECT_COLLISION_ON_MOVEMENT);
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

void UMovementComponentBase::Translate(FVector translation)
{
	Root->AddRelativeLocation(translation, DETECT_COLLISION_ON_MOVEMENT);
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
