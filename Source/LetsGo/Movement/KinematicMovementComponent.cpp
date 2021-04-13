#include "KinematicMovementComponent.h"

#include "Components/ShapeComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

UKinematicMovementComponent::UKinematicMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UKinematicMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	auto const owner = GetOwner();
	auto const rootComponent = owner->GetRootComponent();
	_rootCollider = Cast<UShapeComponent>(rootComponent);
	AssertIsNotNull(_rootCollider);

	_previousLocation = _rootCollider->GetComponentLocation();
	_collisionShape = _rootCollider->GetCollisionShape();
	_collisionQueryParams.AddIgnoredActor(owner);
}

void UKinematicMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto const translationAmount = _speed * DeltaTime;
	auto const forwardDirection = _rootCollider->GetForwardVector();
	auto const translation = forwardDirection * translationAmount;
	auto const rootColliderLocation = _rootCollider->GetComponentLocation();
	auto const rootColliderRotation = _rootCollider->GetComponentQuat();
	
	auto const isBlocked = GetWorld()->SweepSingleByChannel(
		_bufferHitResult,
		_previousLocation,
		rootColliderLocation,
		rootColliderRotation,
		_collisionChannel,
		_collisionShape,
		_collisionQueryParams
	);

	_rootCollider->AddWorldOffset(translation, false);

	if(_bufferHitResult.bBlockingHit)
	{
		Collision.Broadcast(_bufferHitResult);
	}

	_previousLocation = rootColliderLocation;
}
