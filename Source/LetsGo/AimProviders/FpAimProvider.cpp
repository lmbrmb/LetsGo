#include "FpAimProvider.h"

#include "LetsGo/Utils/AssertUtils.h"

FpAimProvider::FpAimProvider(USceneComponent* aimPivot, const float aimRange, ECollisionChannel collisionChannel) :
	_aimPivot(aimPivot),
	_aimRange(aimRange),
	_collisionChannel(collisionChannel)
{
	AssertIsNotNull(_aimPivot);
	AssertIsGreaterOrEqual(_aimRange, 1.0f);
	
	auto const pivotOwner = _aimPivot->GetOwner();
	AssertIsNotNull(pivotOwner);

	_collisionQueryParams.AddIgnoredActor(pivotOwner);
}

FVector FpAimProvider::GetTargetAimLocation()
{
	auto const rayStartLocation = _aimPivot->GetComponentLocation();
	auto const forward = _aimPivot->GetForwardVector();
	auto const rayEndLocation = rayStartLocation + forward * _aimRange;
	auto const isBlockingHit = _aimPivot->GetWorld()->LineTraceSingleByChannel(
		_aimHitResult,
		rayStartLocation,
		rayEndLocation,
		_collisionChannel,
		_collisionQueryParams
	);

	auto const targetAimLocation = isBlockingHit ? _aimHitResult.ImpactPoint : rayEndLocation;

	return targetAimLocation;
}
