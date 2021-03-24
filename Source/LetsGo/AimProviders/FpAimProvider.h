#pragma once

#include "IAimProvider.h"

class FpAimProvider final : public IAimProvider
{
public:
	FpAimProvider(USceneComponent* aimPivot, const float aimRange, ECollisionChannel collisionChannel);

	// IAimProvider implementation
	virtual FVector GetTargetAimLocation() override;

private:
	USceneComponent* _aimPivot;

	float _aimRange;

	FHitResult _aimHitResult;

	ECollisionChannel _collisionChannel;

	FCollisionQueryParams _collisionQueryParams;
};
