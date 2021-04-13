#pragma once

#include "GunShotComponent.h"

#include "ProjectileShotComponent.generated.h"

DECLARE_EVENT_OneParam(UProjectileShotComponent, EShotPerformed_ProjectileShot, const USceneComponent* firePivot);

DECLARE_EVENT_OneParam(UProjectileShotComponent, EProjectileHit_ProjectileShot, FHitResult hitResult)

///<summary>
///Projectile shot component.
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UProjectileShotComponent : public UGunShotComponent
{
	GENERATED_BODY()

public:
	virtual void OnShotRequested(const USceneComponent* firePivot) override;

	EShotPerformed_ProjectileShot ShotPerformed;

	EProjectileHit_ProjectileShot ProjectileHit;
};
