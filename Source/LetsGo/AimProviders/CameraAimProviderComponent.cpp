#include "CameraAimProviderComponent.h"

#include "CameraAimProvider.h"

void UCameraAimProviderComponent::SetCamera(UCameraComponent* cameraComponent)
{
	auto const cameraAimProvider = new CameraAimProvider(cameraComponent, _aimRange, _collisionChannel);
	SetAimProvider(cameraAimProvider);
}
