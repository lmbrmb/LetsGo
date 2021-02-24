#include "LineShotComponent.h"
#include "DrawDebugHelpers.h"
#include "LetsGo/HealthSystem/HealthComponent.h"

void ULineShotComponent::OnShot(USceneComponent* firePivot, USceneComponent* aimProvider)
{
	auto const rayStartLocation = firePivot->GetComponentLocation();
	auto const aimForward = aimProvider->GetForwardVector();
	auto const targetAimLocation = aimProvider->GetComponentLocation() + aimForward * _distance;
	auto const shotDirection = (targetAimLocation - rayStartLocation).GetSafeNormal();
	auto rayEndLocation = rayStartLocation + shotDirection * _distance;

	_collisionQueryParams.AddIgnoredActor(GetOwner());

	auto const isHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		rayStartLocation,
		rayEndLocation,
		ECC_WorldStatic,
		_collisionQueryParams
	);

	if(isHitted)
	{
		rayEndLocation = _hitResult.ImpactPoint;
		auto const healthComponent = _hitResult.Actor->FindComponentByClass<UHealthComponent>();
		if (healthComponent)
		{
			healthComponent->Injure(_damage);
		}
	}
	
	DrawDebugLine(GetWorld(), rayStartLocation, rayEndLocation, FColor::Red, false, 10);
	
	BpOnShot(rayStartLocation, rayEndLocation);
}
