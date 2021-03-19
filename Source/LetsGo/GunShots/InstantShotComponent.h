#pragma once

#include "GunShotComponent.h"

#include "InstantShotComponent.generated.h"

DECLARE_EVENT_TwoParams(UGunShotComponent, EShotPerformed_InstantShot, const USceneComponent* firePivot, const bool isAnyBulletDamaged);
DECLARE_EVENT_TwoParams(UGunShotComponent, EBulletTraced_InstantShot, const bool isDamaged, const FHitResult& hitResult)

///<summary>
///Instant shot component. Line cast. Options: dispersion, damage over distance scaling.
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UInstantShotComponent : public UGunShotComponent
{
	GENERATED_BODY()

public:
	virtual void OnShotRequested(const USceneComponent* firePivot) override;

	EShotPerformed_InstantShot ShotPerformed;

	EBulletTraced_InstantShot BulletTraced;
	
protected:
	virtual void BeginPlay() override;
	
private:
	FHitResult _hitResult;

	FCollisionQueryParams _collisionQueryParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _bulletCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _bulletDamage = 50;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _maxRange = 500;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _aimOffsetRadius = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _damageOverDistanceCurve;

	void ProcessAimLocation(
		FVector& targetAimLocation,
		float& dispersionByDistance
	);
	
	void ProcessBullet(
		const USceneComponent* firePivot,
		const FVector& targetAimLocation,
		const float dispersionByDistance,
		bool& isAnyBulletDamaged
	);

	FVector GetBulletDirection(
		const USceneComponent* firePivot,
		const FVector& bulletStartLocation,
		const FVector& targetAimLocation,
		const float dispersionByDistance
	) const;
	
	void TraceBullet(
		const FVector& rayStartLocation,
		FVector& rayEndLocation,
		bool& isAnyBulletDamaged
	);
	
	float GetBulletDamage() const;

	FVector GetDispersionOffset(const float dispersion, const FVector offsetVector) const;
};
