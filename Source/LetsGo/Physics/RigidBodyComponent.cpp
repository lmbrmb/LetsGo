#include "RigidBodyComponent.h"

#include "LetsGo/Forces/ForceFactory.h"
#include "LetsGo/GameModes/ProjectGameModeBase.h"
#include "LetsGo/Utils/AssertUtils.h"

const FName URigidBodyComponent::GRAVITY_FORCE_ID = "Gravity";

URigidBodyComponent::URigidBodyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URigidBodyComponent::BeginPlay()
{
	Super::BeginPlay();

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const projectGameModeBase = Cast<AProjectGameModeBase>(authGameMode);
	AssertIsNotNull(projectGameModeBase);
	auto const diContainer = projectGameModeBase->GetDiContainer();

	auto const forceFactory = diContainer->GetInstance<ForceFactory>();
	_forceFactory = &forceFactory.Get();

	auto const owner = GetOwner();
	auto const rootComponent = owner->GetRootComponent();
	_rootCollider = Cast<UShapeComponent>(rootComponent);
	AssertIsNotNull(_rootCollider);

	_collisionShape = _rootCollider->GetCollisionShape();
	_collisionQueryParams.bIgnoreTouches = true;
	_collisionQueryParams.AddIgnoredActor(owner);

	if (!FMath::IsNearlyZero(_gravityForceMagnitude))
	{
		auto const gravityForce = _forceFactory->Create(GRAVITY_FORCE_ID, FVector::DownVector, _gravityForceMagnitude);
		_forces.Add(gravityForce);
	}
}

void URigidBodyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ProcessForces(DeltaTime);
	UpdateVelocity();
}

void URigidBodyComponent::ProcessForces(const float deltaTime)
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
	_rootCollider->AddWorldOffset(deltaLocation, true, &_hitResult);

	auto isInAir = true;

	if(_hitResult.bBlockingHit)
	{
		auto const dot = FVector::DotProduct(FVector::UpVector, _hitResult.Normal);
		isInAir = dot < 0.5f;
	}

	if (_isInAir != isInAir)
	{
		_isInAir = isInAir;
		
		if (isInAir)
		{
			_lastGroundTime = GetWorld()->TimeSeconds;
		}
		else
		{
			_forces.RemoveAll([](IForce* f) {return f->GetId() != GRAVITY_FORCE_ID;});
			
			auto const airTime = GetWorld()->TimeSeconds - _lastGroundTime;
			Land.Broadcast(airTime);
		}
	}
}

void URigidBodyComponent::UpdateVelocity()
{
	auto const location = _rootCollider->GetComponentLocation();
	_velocity = location - _previousLocation;
	_previousLocation = location;
}

void URigidBodyComponent::AddForce(const FName& id, const FVector& direction, const float magnitude)
{
	auto const force = _forceFactory->Create(
		id,
		direction,
		magnitude
	);
	_forces.Add(force);
}

void URigidBodyComponent::AddForce(
	const FName& id,
	const FVector& direction,
	UCurveFloat* magnitudeCurve
)
{
	auto const force = _forceFactory->Create(
		id,
		direction,
		magnitudeCurve
	);
	_forces.Add(force);
}

void URigidBodyComponent::AddForce(
	const FName& id,
	const FVector& direction,
	UCurveFloat* magnitudeCurve,
	const float curveMagnitudeMultiplier,
	const float curveTimeMultiplier
)
{
	auto const force = _forceFactory->Create(
		id,
		direction,
		magnitudeCurve,
		curveMagnitudeMultiplier,
		curveTimeMultiplier
	);
	_forces.Add(force);
}

void URigidBodyComponent::RemoveForce(const FName& id)
{
	_forces.RemoveAll([id](IForce* f) {return f->GetId() == id; });
}

bool URigidBodyComponent::GetIsInAir() const
{
	return _isInAir;
}

const FVector& URigidBodyComponent::GetVelocity() const
{
	return _velocity;
}