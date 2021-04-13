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

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	TSubclassOf<AProjectile> _projectileBlueprint;

	void OnProjectileHit(AProjectile* projectile, const FHitResult& hitResult) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _projectileDamage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _damageOverDistanceCurve;
};
