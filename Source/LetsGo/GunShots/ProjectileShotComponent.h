#pragma once

#include "GunShotComponent.h"

#include "LetsGo/Projectiles/Projectile.h"

#include "ProjectileShotComponent.generated.h"

DECLARE_EVENT_OneParam(UProjectileShotComponent, EShotPerformed_ProjectileShot, const USceneComponent* firePivot);

///<summary>
///Projectile shot component.
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UProjectileShotComponent : public UGunShotComponent
{
	GENERATED_BODY()

public:
	virtual void OnShotRequested(const USceneComponent* firePivot) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	TSubclassOf<AProjectile> _projectileBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _maxDamage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _explosionRadius = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _damageOverDistanceCurve;

	UPROPERTY(EditAnywhere, Category = "ImpactForce", meta = (AllowPrivateAccess = "true"))
	float _impactForceCurveMagnitudeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = "ImpactForce", meta = (AllowPrivateAccess = "true"))
	float _impactForceCurveTimeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = "ImpactForce", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _impactForceCurve;

	FCollisionShape _collisionShape;

	TArray<AActor*> _hittedActors;

	FHitResult _hitResult;

	void OnProjectileHit(AProjectile* projectile, const FHitResult& hitResult);

	void OnProjectileLifeTimeExpired(AProjectile* projectile) const;

	bool SimulateExplosion(const FHitResult& collisionHitResult);

	bool IsActorHittedByExplosion(
		const FHitResult& collisionHitResult,
		const FHitResult& explosionHitResult,
		AActor* testActor
	);
	
	float CalculateDamage(
		const FHitResult& collisionHitResult,
		const FHitResult& explosionHitResult,
		AActor* testActor
	) const;

	void ApplyForce(
		const AActor* actor,
		const FVector& explosionEpicenterLocation
	) const;

	static const FName ForceName;
};
