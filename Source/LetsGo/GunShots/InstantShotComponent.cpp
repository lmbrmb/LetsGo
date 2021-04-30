#include "InstantShotComponent.h"

#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/MathUtils.h"

void UInstantShotComponent::BeginPlay()
{
	Super::BeginPlay();

	_collisionQueryParams.bIgnoreTouches = true;
	_collisionQueryParams.AddIgnoredActor(GunActor);
	_collisionQueryParams.AddIgnoredActor(GunOwner);
}

void UInstantShotComponent::OnShotRequested(const USceneComponent* firePivot)
{
	AssertIsNotNull(firePivot);
	AssertIsNotNull(AimProvider);

	auto const startAimLocation = firePivot->GetComponentLocation();
	auto targetAimLocation = AimProvider->GetTargetAimLocation();

	float dispersionByDistance;
	ProcessAimLocation( startAimLocation, targetAimLocation, dispersionByDistance);

	auto isAnyBulletDamaged = false;
	for (auto i = 0; i < _bulletCount; i++)
	{
		ProcessBullet(firePivot, targetAimLocation, dispersionByDistance, isAnyBulletDamaged);
	}

	ShotPerformed.Broadcast(isAnyBulletDamaged);
}

void UInstantShotComponent::ProcessAimLocation(
	const FVector& startAimLocation,
	FVector& targetAimLocation,
	float& dispersionByDistance
)
{
	auto const aimDirection = (targetAimLocation - startAimLocation).GetSafeNormal();
	targetAimLocation = startAimLocation + aimDirection * _maxRange;
	dispersionByDistance = 1.0f;

	auto const isHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		startAimLocation,
		targetAimLocation,
		CollisionChannel,
		_collisionQueryParams
	);

	if (!isHitted)
	{
		return;
	}

	auto const impactPoint = _hitResult.ImpactPoint;
	auto const impactPointDistance = FVector::Distance(targetAimLocation, impactPoint);
	dispersionByDistance = 1.0f - impactPointDistance / _maxRange;
	targetAimLocation = impactPoint;
}

void UInstantShotComponent::ProcessBullet(
	const USceneComponent* firePivot,
	const FVector& targetAimLocation, 
	const float dispersionByDistance,
	bool& isAnyBulletDamaged
)
{
	auto const bulletStartLocation = firePivot->GetComponentLocation();
	auto const shotDirection = GetBulletDirection(firePivot, bulletStartLocation, targetAimLocation, dispersionByDistance);
	auto rayEndLocation = bulletStartLocation + shotDirection * _maxRange;
	TraceBullet(bulletStartLocation, rayEndLocation, isAnyBulletDamaged);
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
	auto const randomRightOffset = firePivot->GetRightVector() * GetDispersionCoefficient(randomPoint.X * dispersionByDistance);
	auto const randomUpOffset = firePivot->GetUpVector() * GetDispersionCoefficient(randomPoint.Y * dispersionByDistance);
	
	auto const aimLocationWithOffset = targetAimLocation + randomRightOffset + randomUpOffset;
	auto const direction = (aimLocationWithOffset - bulletStartLocation).GetSafeNormal();
	
	return direction;
}

void UInstantShotComponent::TraceBullet(
	const FVector& rayStartLocation,
	FVector& rayEndLocation,
	bool& isAnyBulletDamaged
)
{
	auto const isBlockingHit = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		rayStartLocation,
		rayEndLocation,
		CollisionChannel,
		_collisionQueryParams
	);

	auto isDamaged = false;
	
	if (isBlockingHit)
	{
		rayEndLocation = _hitResult.ImpactPoint;

		auto const actorPtr = _hitResult.Actor;
		if (actorPtr.IsValid())
		{
			auto const damageAmount = GetBulletDamage();
			auto const healthComponent = actorPtr->FindComponentByClass<UHealthComponent>();
			if (healthComponent)
			{
				const Damage damage(InstigatorPlayerId, InstigatorWeaponType, _hitResult, damageAmount);
				isDamaged = healthComponent->TryInjure(damage);
				isAnyBulletDamaged |= isDamaged;
			}
		}
	}

	BulletTraced.Broadcast(isDamaged, _hitResult);

	//auto const lineColor = isBlockingHit ? FColor::Red : FColor::Blue;
	//DrawDebugLine(GetWorld(), rayStartLocation, rayEndLocation, lineColor, false, 1);
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

float UInstantShotComponent::GetDispersionCoefficient(const float dispersion) const
{
	auto const maxRandomDistance = dispersion * _aimOffsetRadius;
	auto const randomDistance = FMath::RandRange(0.0f, maxRandomDistance);
	return randomDistance;
}
