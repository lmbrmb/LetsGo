#include "ConeShotComponent.h"

void UConeShotComponent::OnShot(USceneComponent* firePivot, USceneComponent* aimProvider)
{
	auto const startLocation = firePivot->GetComponentLocation();
	auto const rotation = firePivot->GetComponentRotation();
	BpOnShot(startLocation, rotation);
	// TODO: damage logic
}