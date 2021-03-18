#pragma once

#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponId.h"

#include "GunShotComponent.generated.h"

class UAudioComponent;

DECLARE_EVENT_OneParam(UGunShotComponent, EShotPerformed_GunShot, const bool isAnyBulletDamaged);

///<summary>
///Gun shot base component
///</summary>
UCLASS( ClassGroup=(Custom) )
class LETSGO_API UGunShotComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGunShotComponent();

	EShotPerformed_GunShot ShotPerformed;
	
	void SetWeaponId(const WeaponId& weaponId);
	
	void SetPlayerId(const PlayerId& instigatorId);
	
	void SetAimProvider(USceneComponent* aimProvider);

	virtual void OnShotRequested(const USceneComponent* firePivot) {};
	
protected:
	WeaponId InstigatorWeaponId;

	PlayerId InstigatorPlayerId;

	USceneComponent* AimProvider = nullptr;
	
	AActor* GunActor;

	AActor* GunOwner;

	UFUNCTION(BlueprintCallable)
	bool GetIsLocalPlayer() const;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpInit();
	
private:
	bool _isLocalPlayer = false;
	
	// No way to add component in blueprint
	// Audio component is a must have for shot sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Meta = (AllowPrivateAccess = "true"))
	UAudioComponent* _audioComponent;
};
