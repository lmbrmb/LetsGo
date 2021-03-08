#include "InstantShotComponent.h"

#include "DrawDebugHelpers.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/MathUtils.h"

void UInstantShotComponent::Init()
{
	_collisionQueryParams.AddIgnoredActor(GetOwner());
}

void UInstantShotComponent::OnShot(const FGuid& instigatorId, const USceneComponent* firePivot, const USceneComponent* aimProvider)
{
	FVector targetAimLocation;
	float dispersionByDistance;
	ProcessAimLocation(aimProvider, targetAimLocation, dispersionByDistance);
	
	for (auto i = 0; i < _bulletCount; i++)
	{
		ProcessBullet(instigatorId, firePivot, targetAimLocation, dispersionByDistance);
	}
	
	BpOnShot(firePivot);
}

void UInstantShotComponent::ProcessAimLocation(
	const USceneComponent* aimProvider,
	FVector& targetAimLocation,
	float& dispersionByDistance
)
{
	auto const aimStartLocation = aimProvider->GetComponentLocation();
	auto const aimForward = aimProvider->GetForwardVector();
	targetAimLocation = aimStartLocation + aimForward * _maxRange;
	dispersionByDistance = 1.0f;
	
	auto const isHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		aimStartLocation,
		targetAimLocation,
		ECC_WorldStatic,
		_collisionQueryParams
	);

	if (isHitted)
	{
		auto const impactPoint = _hitResult.ImpactPoint;
		auto const impactPointDistance = FVector::Distance(targetAimLocation, impactPoint);
		dispersionByDistance = 1.0f - impactPointDistance / _maxRange;
		targetAimLocation = impactPoint;
	}
}

void UInstantShotComponent::ProcessBullet(
	const FGuid& instigatorId,
	const USceneComponent* firePivot,
	const FVector& targetAimLocation, 
	const float dispersionByDistance
)
{
	auto const rayStartLocation = firePivot->GetComponentLocation();
	auto const shotDirection = GetBulletDirection(firePivot, rayStartLocation, targetAimLocation, dispersionByDistance);
	auto rayEndLocation = rayStartLocation + shotDirection * _maxRange;
	TraceBullet(instigatorId, rayStartLocation, rayEndLocation);
}

FVector UInstantShotComponent::GetBulletDirection(
	const USceneComponent* firePivot,
	const FVector& bulletStartLocation,
	const FVector& targetAimLocation,
	const float dispersionByDistance
) const
{
	//No dispersion
	if(_aimOffsetRadius <= 0)
	{
		return (targetAimLocation - bulletStartLocation).GetSafeNormal();
	}

	auto const randomPoint = MathUtils::GetRandomPointOnUnitCircle();
	auto const randomRightOffset = GetDispersionOffset(randomPoint.X * dispersionByDistance, firePivot->GetRightVector());
	auto const randomUpOffset = GetDispersionOffset(randomPoint.Y * dispersionByDistance, firePivot->GetUpVector());
	
	auto const aimLocationWithOffset = targetAimLocation + randomRightOffset + randomUpOffset;
	auto const direction = (aimLocationWithOffset - bulletStartLocation).GetSafeNormal();
	
	return direction;
}

void UInstantShotComponent::TraceBullet(const FGuid& instigatorId, const FVector& rayStartLocation, FVector& rayEndLocation)
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
			auto const damageAmount = GetBulletDamage();
			auto const healthComponent = actorPtr->FindComponentByClass<UHealthComponent>();
			if (healthComponent)
			{
				healthComponent->Injure(Damage(instigatorId, damageAmount));
			}
		}
	}

	auto const lineColor = isHitted ? FColor::Red : FColor::Blue;
	DrawDebugLine(GetWorld(), rayStartLocation, rayEndLocation, lineColor, false, 10);
	BpOnBullet(_hitResult);
}

float UInstantShotComponent::GetBulletDamage() const
{
	if (!_damageOverDistanceCurve)
	{
		return _bulletDamage;
	};
	
	auto const distancePercent = _hitResult.Distance / _maxRange;
	auto const distanceModifier = _damageOverDistanceCurve->GetFloatValue(distancePercent);
	auto const resultingDamage = FMath::RoundToInt(_bulletDamage * distanceModifier) ;
	return resultingDamage;
}

FVector UInstantShotComponent::GetDispersionOffset(const float dispersion, const FVector offsetVector) const
{
	auto const maxRandomDistance = dispersion * _aimOffsetRadius;
	auto const randomDistance = FMath::RandRange(0.0f, maxRandomDistance);
	auto const randomOffset = randomDistance * offsetVector;
	return randomOffset;
}
