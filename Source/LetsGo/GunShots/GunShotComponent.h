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

	void SetShotTraceOrigin(USceneComponent* shotTraceOrigin);

	virtual void OnShotRequested() {};

	EShotPerformed_GunShot ShotPerformed;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Pawn;
	
	WeaponType InstigatorWeaponType;

	PlayerId InstigatorPlayerId;

	IAimProvider* AimProvider = nullptr;

	USceneComponent* ShotTraceOrigin = nullptr;

	AActor* GunActor = nullptr;

	AActor* GunOwner = nullptr;

	virtual void BeginPlay() override;
};
