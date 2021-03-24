#pragma once

#include "Components/ActorComponent.h"
#include "IGun.h"
#include "LetsGo/AssetFactories/GunFactory.h"
#include "AmmoProvider.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponId.h"
#include "LetsGo/Items/AmmoItem.h"
#include "LetsGo/Items/AmmoItemFactory.h"
#include "LetsGo/Items/GunItem.h"
#include "LetsGo/Items/GunItemFactory.h"
#include "LetsGo/Items/IItemProcessor.h"
#include "LetsGo/Items/Item.h"

#include "WeaponManagerComponent.generated.h"

DECLARE_EVENT_ThreeParams(UWeaponManagerComponent, EShotPerformed_UWeaponManagerComponent, const PlayerId& playerId, const WeaponId& gunId, const bool isAnyBulletDamaged);

DECLARE_EVENT(UWeaponManagerComponent, EWeaponChanged_UWeaponManagerComponent);

UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UWeaponManagerComponent final : public UActorComponent, public IItemProcessor
{
	GENERATED_BODY()

public:
	UWeaponManagerComponent();

	void StartFire();

	void StopFire();

	void Reload();

	void NextWeapon();

	void PreviousWeapon();

	void ChangeWeapon(const int indexModifier);

	void ChangeWeaponPivot();

	UFUNCTION(BlueprintCallable)
	void AddWeaponPivot(USceneComponent* weaponPivot);

	UFUNCTION(BlueprintCallable)
	void SetAimProvider(USceneComponent* aimProvider);

	UFUNCTION(BlueprintCallable)
	void SetOwnerSkeletalMeshComponent(USkeletalMeshComponent* ownerSkeletalMeshComponent);
	
	void SetPlayerId(const PlayerId& playerId);
	
	virtual bool TryProcessItem(Item* item) override;

	IWeapon* GetCurrentWeapon() const;

	IGun* GetCurrentGun() const;

	EShotPerformed_UWeaponManagerComponent ShotPerformed;

	EWeaponChanged_UWeaponManagerComponent WeaponChanged;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnGunShotPerformed(const bool isAnyBulletDamaged);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpOnWeaponChanged();
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _startWeaponId;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	bool _shouldEquipWeaponOnPickup;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName WeaponSocketName = "RightHandWeaponSocket";
	
	const int UNDEFINED_INDEX = -1;

	bool _isInitialized = false;

	bool _isFireStarted;
	
	PlayerId _playerId;

	USceneComponent* _aimProvider;

	TArray<USceneComponent*> _weaponPivots;

	USceneComponent* _weaponPivot = nullptr;

	int _weaponPivotIndex = UNDEFINED_INDEX;

	GunItemFactory* _gunItemFactory;
	
	GunFactory* _gunFactory;

	AmmoItemFactory* _ammoItemFactory;

	TArray<IWeapon*> _weapons;
	
	TArray<AActor*> _weaponActors;

	/// <summary>
	/// Gun Id / Ammo provider
	/// </summary>
	TMap<FName, AmmoProvider*> _ammoProviders;

	USkeletalMeshComponent* _ownerSkeletalMeshComponent;
	
	AActor* _weaponActor = nullptr;

	IWeapon* _weapon = nullptr;

	IGun* _gun = nullptr;

	int _weaponIndex = UNDEFINED_INDEX;

	void EquipWeapon(const int weaponIndex);

	TArray<TFunction<bool(Item*)>> _itemProcessors;

	bool TryProcessItemAsGun(Item* item);

	bool TryProcessItemAsAmmo(Item* item);

	AmmoProvider* GetAmmoProvider(const FName ammoId);

	AmmoProvider* CreateAmmoProvider(const GunItem* gunItem);

	AmmoProvider* CreateAmmoProvider(const AmmoItem* ammoItem);

	AActor* CreateGun(const GunItem* gunItem);

	void AttachWeapon(AActor* weaponActor) const;

	bool CanAttachWeapon() const;

	void OnPartialInitialization();

	void CreateStartWeapon();

	void OnGunShotPerformed(const IGun* gun, const bool isAnyBulletDamaged);
};
