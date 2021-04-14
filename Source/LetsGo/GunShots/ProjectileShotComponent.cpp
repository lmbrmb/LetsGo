#include "ProjectileShotComponent.h"

#include "DrawDebugHelpers.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/AssetUtils.h"

void UProjectileShotComponent::OnShotRequested(const USceneComponent* firePivot)
{
	AssertIsNotNull(firePivot);
	AssertIsNotNull(_projectileBlueprint);

	auto const projectile = AssetUtils::SpawnBlueprint<AProjectile>(
		GetWorld(),
		nullptr,
		_projectileBlueprint,
		firePivot->GetComponentTransform(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	projectile->Hit.AddUObject(this, &UProjectileShotComponent::OnProjectileHit);
	projectile->LifeTimeExpired.AddUObject(this, &UProjectileShotComponent::OnProjectileLifeTimeExpired);
}

void UProjectileShotComponent::BeginPlay()
{
	Super::BeginPlay();

	_collisionShape.SetSphere(_explosionRadius);
}

void UProjectileShotComponent::OnProjectileHit(AProjectile* projectile, const FHitResult& hitResult) const
{
	AssertIsTrue(hitResult.bBlockingHit > 0);
	auto const isAnyBulletDamaged = false;
	auto const explosionEpicenterLocation = hitResult.ImpactPoint;
	
	TArray<FHitResult> explosionHitResults;
	auto const isHitted = GetWorld()->SweepMultiByChannel(
		explosionHitResults,
		explosionEpicenterLocation,
		explosionEpicenterLocation,
		FQuat::Identity,
		_explosionChannel,
		_collisionShape
	);

	//DrawDebugSphere(GetWorld(), explosionEpicenterLocation, _explosionRadius, 12, FColor::Red, false, 2);
	
	if(!isHitted)
	{
		return;
	}

	TArray<AActor*> hittedActors;
	
	for (auto explosionHitResult : explosionHitResults)
	{
		if (!explosionHitResult.Actor.IsValid())
		{
			continue;
		}
		
		auto const hitActor = explosionHitResult.Actor.Get();

		if(hittedActors.Contains(hitActor))
		{
			continue;
		}
		
		hittedActors.Add(hitActor);
		
		auto const healthComponent = hitActor->FindComponentByClass<UHealthComponent>();

		if (!healthComponent)
		{
			continue;
		}

		// TODO: Check walls

		auto const distance = FVector::Distance(explosionHitResult.ImpactPoint, explosionEpicenterLocation);
		
		auto const damageAmount = GetDamage(distance);
		const Damage damage(InstigatorPlayerId, InstigatorWeaponType, damageAmount);
		healthComponent->TryInjure(damage);
	}

	ShotPerformed.Broadcast(isAnyBulletDamaged);
}

void UProjectileShotComponent::OnProjectileLifeTimeExpired(AProjectile* projectile) const
{
	ShotPerformed.Broadcast(false);
}

float UProjectileShotComponent::GetDamage(const float distance) const
{
	if (!_damageOverDistanceCurve)
	{
		return _maxDamage;
	};

	AssertIsLessOrEqual(distance, _explosionRadius, _maxDamage);
	
	auto const distancePercent = distance / _explosionRadius;
	auto const distanceModifier = _damageOverDistanceCurve->GetFloatValue(distancePercent);
	auto const resultingDamage = FMath::RoundToInt(_maxDamage * distanceModifier);
	
	return resultingDamage;
}
