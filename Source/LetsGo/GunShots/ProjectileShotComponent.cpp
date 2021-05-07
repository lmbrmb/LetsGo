#include "ProjectileShotComponent.h"

#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Movement/KinematicMovementComponent.h"
#include "LetsGo/Physics/RigidBodyComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/AssetUtils.h"

const FName UProjectileShotComponent::ForceName = FName("Explosion");

void UProjectileShotComponent::OnShotRequested()
{
	AssertIsNotNull(ShotTraceOrigin);
	AssertIsNotNull(_projectileBlueprint);

	auto const spawnLocation = ShotTraceOrigin->GetComponentLocation();
	auto const targetAimLocation = AimProvider->GetTargetAimLocation();
	auto const aimDirection = (targetAimLocation - spawnLocation).GetSafeNormal();
	auto const rotation = UKismetMathLibrary::MakeRotFromX(aimDirection);

	auto const projectile = AssetUtils::SpawnBlueprint<AProjectile>(
		GetWorld(),
		nullptr,
		_projectileBlueprint,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		spawnLocation,
		rotation
		);

	projectile->Hit.AddUObject(this, &UProjectileShotComponent::OnProjectileHit);
	projectile->LifeTimeExpired.AddUObject(this, &UProjectileShotComponent::OnProjectileLifeTimeExpired);

	auto const kinematicMovementComponent = projectile->FindComponentByClass<UKinematicMovementComponent>();
	if(kinematicMovementComponent)
	{
		kinematicMovementComponent->AddIgnoredActor(GunActor);
		kinematicMovementComponent->AddIgnoredActor(GunOwner);
	}
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

	_explosionHitResults.Empty();
	auto const isExplosionHittedSomething = GetWorld()->SweepMultiByChannel(
		_explosionHitResults,
		explosionEpicenterLocation,
		explosionEpicenterLocation,
		FQuat::Identity,
		CollisionChannel,
		_collisionShape
	);

	//DrawDebugSphere(GetWorld(), explosionEpicenterLocation, _explosionRadius, 24, FColor::Red, false, 2);

	if (!isExplosionHittedSomething)
	{
		return false;
	}

	auto isExplosionDamaged = false;
	
	for (auto explosionHitResult : _explosionHitResults)
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

		auto const collisionActor = collisionHitResult.Actor;
		auto const isDirectHit = collisionActor.IsValid() && collisionActor.Get() == hittedActor;

		auto const actorHitLocation = explosionHitResult.ImpactPoint;
		auto const distance = FVector::Distance(explosionEpicenterLocation, actorHitLocation);
		auto const explosionDistancePercent = distance / _explosionRadius;

		auto const damageAmount = CalculateDamage(isDirectHit, explosionDistancePercent);

		const Damage damage(InstigatorPlayerId, InstigatorWeaponType, collisionHitResult, damageAmount);
		auto const isDamaged = healthComponent->TryInjure(damage);
		isExplosionDamaged |= isDamaged;

		ApplyForce(
			isDirectHit,
			hittedActor,
			explosionEpicenterLocation,
			explosionDistancePercent
		);
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
		CollisionChannel
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
	const bool isDirectHit,
	const float explosionDistancePercent
) const
{
	if (isDirectHit)
	{
		return _directHitDamage;
	}

	if (!_areaDamageOverDistanceCurve)
	{
		return _maxAreaDamage;
	};
	
	auto const distanceModifier = _areaDamageOverDistanceCurve->GetFloatValue(explosionDistancePercent);
	auto const resultingDamage = FMath::RoundToInt(_maxAreaDamage * distanceModifier);
	
	return resultingDamage;
}

void UProjectileShotComponent::ApplyForce(
	const bool isDirectHit,
	const AActor* actor,
	const FVector& explosionEpicenterLocation,
	const float explosionDistancePercent
) const
{
	if (!_impactForceCurve)
	{
		return;
	}

	auto const rigidBodyComponent = actor->FindComponentByClass<URigidBodyComponent>();
	if (!rigidBodyComponent)
	{
		return;
	}

	auto const distanceMultiplier = isDirectHit ? 1.0f : 1 - explosionDistancePercent;

	//DrawDebugSphere(GetWorld(), explosionEpicenterLocation, 20, 10, FColor::Red, false, 5);
	auto const actorDirection = (actor->GetActorLocation() - explosionEpicenterLocation).GetSafeNormal();
	auto const projectedActorDirection = FVector::VectorPlaneProject(actorDirection, FVector::UpVector);
	auto const forceDirection = (projectedActorDirection + FVector::UpVector).GetSafeNormal();

	auto const impactForceCurveMagnitudeMultiplier = _impactForceCurveMagnitudeMultiplier * distanceMultiplier;
	rigidBodyComponent->AddForce(
		ForceName,
		forceDirection,
		_impactForceCurve,
		impactForceCurveMagnitudeMultiplier,
		_impactForceCurveTimeMultiplier
	);
}
