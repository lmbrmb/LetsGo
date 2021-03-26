#include "CameraAimProvider.h"

#include "Camera/CameraComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

CameraAimProvider::CameraAimProvider(UCameraComponent* cameraComponent, const float aimRange, const ECollisionChannel collisionChannel) :
	_cameraComponent(cameraComponent),
	_aimRange(aimRange),
	_collisionChannel(collisionChannel)
{
	AssertIsNotNull(_cameraComponent);
	AssertIsGreaterOrEqual(_aimRange, 1.0f);
	
	auto const cameraOwner = _cameraComponent->GetOwner();
	AssertIsNotNull(cameraOwner);

	_collisionQueryParams.AddIgnoredActor(cameraOwner);
}

FVector CameraAimProvider::GetTargetAimLocation()
{
	auto const rayStartLocation = _cameraComponent->GetComponentLocation();
	auto const forward = _cameraComponent->GetForwardVector();
	auto const rayEndLocation = rayStartLocation + forward * _aimRange;
	auto const isBlockingHit = _cameraComponent->GetWorld()->LineTraceSingleByChannel(
		_aimHitResult,
		rayStartLocation,
		rayEndLocation,
		_collisionChannel,
		_collisionQueryParams
	);

	auto const targetAimLocation = isBlockingHit ? _aimHitResult.ImpactPoint : rayEndLocation;

	return targetAimLocation;
}
