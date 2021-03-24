#pragma once

#include "Components/ActorComponent.h"
#include "LetsGo/AimProviders/IAimProvider.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponId.h"

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
	
	void SetWeaponId(const WeaponId& weaponId);
	
	void SetPlayerId(const PlayerId& instigatorId);
	
	void SetAimProvider(IAimProvider* aimProvider);

	virtual void OnShotRequested(const USceneComponent* firePivot) {};
	
protected:
	WeaponId InstigatorWeaponId;

	PlayerId InstigatorPlayerId;

	IAimProvider* AimProvider = nullptr;
	
	AActor* GunActor;

	AActor* GunOwner;

	UFUNCTION(BlueprintCallable)
	bool GetIsLocalPlayer() const;

	virtual void BeginPlay() override;
	
private:
	bool _isLocalPlayer = false;
};
