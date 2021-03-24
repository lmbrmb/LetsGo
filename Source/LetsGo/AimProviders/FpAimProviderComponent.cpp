#include "FpAimProviderComponent.h"

#include "FpAimProvider.h"

void UFpAimProviderComponent::SetAimPivot(USceneComponent* aimPivot)
{
	auto const fpAimProvider = new FpAimProvider(aimPivot, _aimRange, _collisionChannel);
	SetAimProvider(fpAimProvider);
}
