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

	void OnProjectileHit(AProjectile* projectile, const FHitResult& hitResult) const;

	void OnProjectileLifeTimeExpired(AProjectile* projectile) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _maxDamage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _explosionRadius = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _damageOverDistanceCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> _explosionChannel;

	float _explosionRadiusSquared = _explosionRadius * _explosionRadius;

	FCollisionShape _collisionShape;

	float GetDamage(const float distance) const;
};
