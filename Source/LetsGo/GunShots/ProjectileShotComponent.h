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
	float _directHitDamage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _maxAreaDamage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _areaDamageOverDistanceCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _explosionRadius = 300;

	UPROPERTY(EditAnywhere, Category = "ImpactForce", meta = (AllowPrivateAccess = "true"))
	float _impactForceCurveMagnitudeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = "ImpactForce", meta = (AllowPrivateAccess = "true"))
	float _impactForceCurveTimeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = "ImpactForce", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _impactForceCurve;

	FCollisionShape _collisionShape;

	TArray<AActor*> _hittedActors;

	TArray<FHitResult> _explosionHitResults;

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
		const bool isDirectHit,
		const float explosionDistancePercent
	) const;

	void ApplyForce(
		const bool isDirectHit,
		const AActor* actor,
		const FVector& explosionEpicenterLocation,
		const float explosionDistancePercent
	) const;

	static const FName ForceName;
};
