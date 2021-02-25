#include "InstantShotComponent.h"

#include "DrawDebugHelpers.h"
#include "LetsGo/HealthSystem/HealthComponent.h"

void UInstantShotComponent::Init()
{
	_collisionQueryParams.AddIgnoredActor(GetOwner());
}

void UInstantShotComponent::OnShot(USceneComponent* firePivot, USceneComponent* aimProvider)
{
	for (auto i = 0; i < _bulletCount; i++)
	{
		ProcessBullet(firePivot, aimProvider);
	}
}

void UInstantShotComponent::ProcessBullet(USceneComponent* firePivot, USceneComponent* aimProvider)
{
	auto const rotation = firePivot->GetComponentRotation();
	auto const rayStartLocation = firePivot->GetComponentLocation();
	auto const aimForward = aimProvider->GetForwardVector();
	auto const targetAimLocation = aimProvider->GetComponentLocation() + aimForward * _distance;
	auto const shotDirection = GetBulletDirection(rayStartLocation, targetAimLocation);
	auto rayEndLocation = rayStartLocation + shotDirection * _distance;
	TraceBullet(rayStartLocation, rayEndLocation);
	BpOnShot(rayStartLocation, rayEndLocation, rotation);
}

FVector UInstantShotComponent::GetBulletDirection(const FVector& rayStartLocation, const FVector& targetAimLocation )
{
	return (targetAimLocation - rayStartLocation).GetSafeNormal();
}

void UInstantShotComponent::TraceBullet(const FVector& rayStartLocation, FVector& rayEndLocation)
{
	auto const isHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		rayStartLocation,
		rayEndLocation,
		ECC_WorldStatic,
		_collisionQueryParams
	);

	if (isHitted)
	{
		rayEndLocation = _hitResult.ImpactPoint;

		auto const actorPtr = _hitResult.Actor;
		if (actorPtr.IsValid())
		{
			auto const damage = GetBulletDamage();
			auto const healthComponent = actorPtr->FindComponentByClass<UHealthComponent>();
			if (healthComponent)
			{
				healthComponent->Injure(damage);
			}
		}
	}

	DrawDebugLine(GetWorld(), rayStartLocation, rayEndLocation, FColor::Red, false, 10);
}

float UInstantShotComponent::GetBulletDamage() const
{
	if (!_damageOverDistanceCurve)
	{
		return _bulletDamage;
	};
	
	auto const distancePercent = _hitResult.Distance / _distance;
	auto const distanceModifier = _damageOverDistanceCurve->GetFloatValue(distancePercent);
	return _bulletDamage * distanceModifier;
}
