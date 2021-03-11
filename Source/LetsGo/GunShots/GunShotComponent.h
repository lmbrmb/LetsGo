#pragma once

#include "Components/ActorComponent.h"

#include "GunShotComponent.generated.h"

///<summary>
///Gun shot base component
///</summary>
UCLASS( ClassGroup=(Custom) )
class LETSGO_API UGunShotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGunShotComponent();

	void SetWeaponId(const FName& weaponId);
	
	void SetPlayerId(const int32 instigatorId);
	
	void SetAimProvider(USceneComponent* aimProvider);

	virtual void OnShot(const USceneComponent* firePivot) {};
	
protected:
	FName WeaponId;

	int32 PlayerId;

	USceneComponent* AimProvider = nullptr;
};
