#pragma once

#include "IAimProvider.h"
#include "Camera/CameraComponent.h"

class CameraAimProvider final : public IAimProvider
{
public:
	CameraAimProvider(UCameraComponent* cameraComponent, const float aimRange, ECollisionChannel collisionChannel);

	// IAimProvider implementation
	virtual FVector GetTargetAimLocation() override;

private:
	UCameraComponent* _cameraComponent;

	float _aimRange;

	FHitResult _aimHitResult;

	ECollisionChannel _collisionChannel;

	FCollisionQueryParams _collisionQueryParams;
};
