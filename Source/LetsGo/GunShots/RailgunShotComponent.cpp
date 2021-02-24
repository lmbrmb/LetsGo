#include "RailgunShotComponent.h"
#include "DrawDebugHelpers.h"

void URailgunShotComponent::OnShot(USceneComponent* firePivot, USceneComponent* aimProvider)
{
	auto const pivotLocation = firePivot->GetComponentLocation();
	auto const aimForward = aimProvider->GetForwardVector();
	auto const targetAimLocation = aimProvider->GetComponentLocation() + aimForward * _rayDistance;
	auto const shotDirection = (targetAimLocation - pivotLocation).GetSafeNormal();
	auto const rayEndLocation = pivotLocation + shotDirection * _rayDistance;
	DrawDebugLine(GetWorld(), pivotLocation, rayEndLocation, FColor::Red, false, 10);
	BpOnShot(pivotLocation, rayEndLocation);
}
