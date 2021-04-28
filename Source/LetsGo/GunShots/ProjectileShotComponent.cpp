#include "ProjectileShotComponent.h"

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
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		firePivot->GetComponentTransform()
		);

	projectile->Hit.AddUObject(this, &UProjectileShotComponent::OnProjectileHit);
	projectile->LifeTimeExpired.AddUObject(this, &UProjectileShotComponent::OnProjectileLifeTimeExpired);
}

void UProjectileShotComponent::BeginPlay()
{
	Super::BeginPlay();

	_collisionShape.SetSphere(_explosionRadius);
}

void UProjectileShotComponent::OnProjectileHit(AProjectile* projectile, const FHitResult& hitResult)
{
	auto const isExplosionDamaged = SimulateExplosion(hitResult);
	ShotPerformed.Broadcast(isExplosionDamaged);
}

void UProjectileShotComponent::OnProjectileLifeTimeExpired(AProjectile* projectile) const
{
	ShotPerformed.Broadcast(false);
}

bool UProjectileShotComponent::SimulateExplosion(const FHitResult& collisionHitResult)
{
	AssertIsTrue(collisionHitResult.bBlockingHit > 0, false);
	
	_hittedActors.Empty();

	auto const explosionEpicenterLocation = collisionHitResult.ImpactPoint;

	TArray<FHitResult> explosionHitResults;
	auto const isExplosionHittedSomething = GetWorld()->SweepMultiByChannel(
		explosionHitResults,
		explosionEpicenterLocation,
		explosionEpicenterLocation,
		FQuat::Identity,
		_explosionChannel,
		_collisionShape
	);

	//DrawDebugSphere(GetWorld(), explosionEpicenterLocation, _explosionRadius, 24, FColor::Red, false, 2);

	if (!isExplosionHittedSomething)
	{
		return false;
	}

	auto isExplosionDamaged = false;
	
	for (auto explosionHitResult : explosionHitResults)
	{
		if (!explosionHitResult.Actor.IsValid())
		{
			continue;
		}

		auto const hittedActor = explosionHitResult.Actor.Get();

		if (_hittedActors.Contains(hittedActor))
		{
			continue;
		}

		_hittedActors.Add(hittedActor);

		auto const healthComponent = hittedActor->FindComponentByClass<UHealthComponent>();

		if (!healthComponent)
		{
			continue;
		}

		auto const isActorHitted = IsActorHittedByExplosion(
			collisionHitResult,
			explosionHitResult,
			hittedActor
		);

		if (!isActorHitted)
		{
			continue;
		}

		auto const damageAmount = CalculateDamage(collisionHitResult, explosionHitResult, hittedActor);
		const Damage damage(InstigatorPlayerId, InstigatorWeaponType, collisionHitResult, damageAmount);
		auto const isDamaged = healthComponent->TryInjure(damage);

		ApplyForce(hittedActor, explosionHitResult.Normal);

		isExplosionDamaged |= isDamaged;
	}

	return isExplosionDamaged;
}

/// <summary>
/// Checks if actor is hitted.
/// </summary>
/// <returns>
/// "true" if direct hit or no obstacles between explosion epicenter and actor.
/// "false" otherwise
/// </returns>
bool UProjectileShotComponent::IsActorHittedByExplosion(
	const FHitResult& collisionHitResult,
	const FHitResult& explosionHitResult,
	AActor* testActor
)
{
	AssertIsTrue(explosionHitResult.bBlockingHit > 0, false);

	auto const explosionEpicenterLocation = collisionHitResult.ImpactPoint;
	auto const collisionActor = collisionHitResult.Actor;

	if(collisionActor.IsValid() && collisionActor.Get() == testActor)
	{
		return true;
	}

	auto const impactNormalOffset = collisionHitResult.Normal * 10;
	auto const lineStartLocation = explosionEpicenterLocation + impactNormalOffset;
	auto const lineEndLocation = explosionHitResult.ImpactPoint;
	
	//DrawDebugLine(GetWorld(), lineStartLocation, lineEndLocation, FColor::Emerald, false, 10);
	//DrawDebugSphere(GetWorld(), lineStartLocation, 20, 24, FColor::Emerald, false, 10);
	//DrawDebugSphere(GetWorld(), lineEndLocation, 20, 24, FColor::Emerald, false, 10);
	
	auto const isAnythingHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		lineStartLocation,
		lineEndLocation,
		_explosionChannel
	);

	if(!isAnythingHitted)
	{
		return false;
	}
	
	if (_hitResult.Actor.IsValid() && _hitResult.Actor.Get() == testActor)
	{
		return true;
	}

	return false;
}

float UProjectileShotComponent::CalculateDamage(
	const FHitResult& collisionHitResult,
	const FHitResult& explosionHitResult,
	AActor* testActor
) const
{
	if (!_damageOverDistanceCurve)
	{
		return _maxDamage;
	};

	auto const collisionActor = collisionHitResult.Actor;
	
	if (collisionActor.IsValid() && collisionActor.Get() == testActor)
	{
		return _maxDamage;
	}
	
	auto const explosionEpicenterLocation = collisionHitResult.ImpactPoint;
	auto const actorHitLocation = explosionHitResult.ImpactPoint;
	auto const distance = FVector::Distance(explosionEpicenterLocation, actorHitLocation);

	AssertIsLessOrEqual(distance, _explosionRadius, _maxDamage);
	
	auto const distancePercent = distance / _explosionRadius;
	auto const distanceModifier = _damageOverDistanceCurve->GetFloatValue(distancePercent);
	auto const resultingDamage = FMath::RoundToInt(_maxDamage * distanceModifier);
	
	return resultingDamage;
}
