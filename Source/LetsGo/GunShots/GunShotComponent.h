#pragma once

#include "Components/ActorComponent.h"
#include "LetsGo/Data/PlayerId.h"

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

	EShotPerformed_GunShot ShotPerformed;
	
	void SetWeaponId(const FName& weaponId);
	
	void SetPlayerId(const PlayerId& instigatorId);
	
	void SetAimProvider(USceneComponent* aimProvider);

	virtual void OnShotRequested(const USceneComponent* firePivot) {};
	
protected:
	FName WeaponId;

	PlayerId InstigatorId;

	USceneComponent* AimProvider = nullptr;
};
