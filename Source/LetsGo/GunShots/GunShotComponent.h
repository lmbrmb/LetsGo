#pragma once

#include "Components/ActorComponent.h"
#include "LetsGo/AimProviders/IAimProvider.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponType.h"

#include "GunShotComponent.generated.h"

DECLARE_EVENT_OneParam(UGunShotComponent, EShotPerformed_GunShot, const bool isAnyBulletDamaged);

///<summary>
///Gun shot base component
///</summary>
UCLASS( ClassGroup=(Custom) )
class LETSGO_API UGunShotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGunShotComponent();

	void SetWeaponType(const WeaponType& weaponType);

	void SetPlayerId(const PlayerId& instigatorId);

	void SetAimProvider(IAimProvider* aimProvider);

	virtual void OnShotRequested(const USceneComponent* firePivot) {};

	EShotPerformed_GunShot ShotPerformed;

protected:
	UPROPERTY(EditAnywhere, Category = "ImpactForce", meta = (AllowPrivateAccess = "true"))
	float ImpactForceCurveMagnitudeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = "ImpactForce", meta = (AllowPrivateAccess = "true"))
	float ImpactForceCurveTimeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = "ImpactForce", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ImpactForceCurve;
	
	WeaponType InstigatorWeaponType;

	PlayerId InstigatorPlayerId;

	IAimProvider* AimProvider = nullptr;

	AActor* GunActor = nullptr;

	AActor* GunOwner = nullptr;

	virtual void BeginPlay() override;

	void ApplyForce(AActor* actor, const FVector& direction) const;

	static const FName ForceName;
};
