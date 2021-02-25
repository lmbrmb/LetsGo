#pragma once

#include "GunShotComponent.h"
#include "InstantShotComponent.generated.h"

///<summary>
///Instant shot component. Line cast. Options: dispersion, damage over distance scaling.
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UInstantShotComponent : public UGunShotComponent
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

	virtual void OnShot(USceneComponent* firePivot, USceneComponent* aimProvider) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnShot(FVector startPosition, FVector endLocation, FRotator rotation);
	
private:
	FHitResult _hitResult;

	FCollisionQueryParams _collisionQueryParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _bulletCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _bulletDamage = 50;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _distance = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _damageOverDistanceCurve;

	void ProcessBullet(USceneComponent* firePivot, USceneComponent* aimProvider);

	FVector GetBulletDirection(const FVector& rayStartLocation, const FVector& targetAimLocation);
	
	void TraceBullet(const FVector& rayStartLocation, FVector& rayEndLocation);
	
	float GetBulletDamage() const;
};
