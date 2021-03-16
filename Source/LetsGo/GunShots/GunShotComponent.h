#pragma once

#include "Components/ActorComponent.h"

#include "GunShotComponent.generated.h"

DECLARE_EVENT_OneParam(UGunShotComponent, EShotPerformed_UGunShotComponent, const bool isHitted);

///<summary>
///Gun shot base component
///</summary>
UCLASS( ClassGroup=(Custom) )
class LETSGO_API UGunShotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGunShotComponent();

	EShotPerformed_UGunShotComponent ShotPerformed;
	
	void SetWeaponId(const FName& weaponId);
	
	void SetPlayerId(const int32 instigatorId);
	
	void SetAimProvider(USceneComponent* aimProvider);

	virtual void OnShotRequested(const USceneComponent* firePivot) {};
	
protected:
	FName WeaponId;

	int32 PlayerId;

	USceneComponent* AimProvider = nullptr;
};
