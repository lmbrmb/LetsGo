#pragma once

#include "Components/ActorComponent.h"
#include "IGun.h"
#include "LetsGo/AssetFactories/GunFactory.h"
#include "AmmoProvider.h"
#include "LetsGo/AimProviders/IAimProvider.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Items/AmmoItem.h"
#include "LetsGo/Items/AmmoItemFactory.h"
#include "LetsGo/Items/GunItem.h"
#include "LetsGo/Items/GunItemFactory.h"
#include "LetsGo/Items/IItemProcessor.h"
#include "LetsGo/Items/Item.h"

#include "WeaponManagerComponent.generated.h"

DECLARE_EVENT_ThreeParams(UWeaponManagerComponent, EShotPerformed_UWeaponManagerComponent, const PlayerId& playerId, const WeaponType& weaponType, const bool isAnyBulletDamaged);

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

	void HolsterWeapon();

	void NextWeapon();

	void PreviousWeapon();

	bool ChangeWeapon(const int indexModifier);

	void ChangeWeaponPivot();

	void SetAimProvider(IAimProvider* aimProvider);

	UFUNCTION(BlueprintCallable)
	void AddWeaponPivot(USceneComponent* weaponPivot);

	UFUNCTION(BlueprintCallable)
	int GetWeaponsCount() const;
	
	UFUNCTION(BlueprintCallable)
	void SetOwnerSkeletalMeshComponent(USkeletalMeshComponent* ownerSkeletalMeshComponent);
	
	void SetPlayerId(const PlayerId& playerId);
	
	virtual bool TryProcessItem(Item* item) override;

	IWeapon* GetCurrentWeapon() const;

	IGun* GetCurrentGun() const;

	EShotPerformed_UWeaponManagerComponent ShotPerformed;

	EWeaponChanged_UWeaponManagerComponent WeaponEquipped;

	EWeaponChanged_UWeaponManagerComponent WeaponHolstered;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnGunShotPerformed(const bool isAnyBulletDamaged);

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnWeaponEquipped();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnWeaponHolstered();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnOutOfAmmo();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _startWeaponId;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	bool _shouldEquipWeaponOnPickup;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _weaponSocketName = "RightHandWeaponSocket";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _weaponEquipDuration = 0;

	const int UNDEFINED_INDEX = -1;

	const int NO_WEAPON_INDEX = UNDEFINED_INDEX;

	bool _isInitialized = false;

	bool _isFireStarted;
	
	PlayerId _playerId;

	IAimProvider* _aimProvider;

	TArray<USceneComponent*> _weaponPivots;

	USceneComponent* _weaponPivot = nullptr;

	int _weaponPivotIndex = UNDEFINED_INDEX;

	GunItemFactory* _gunItemFactory;
	
	GunFactory* _gunFactory;

	AmmoItemFactory* _ammoItemFactory;

	TArray<AActor*> _weaponActors;

	TArray<IWeapon*> _weapons;

	TArray<IGun*> _guns;
	
	AActor* _weaponActor = nullptr;

	IWeapon* _weapon = nullptr;

	IGun* _gun = nullptr;
	
	/// <summary>
	/// Ammo Id / Ammo provider
	/// </summary>
	TMap<FName, AmmoProvider*> _ammoProviders;

	USkeletalMeshComponent* _ownerSkeletalMeshComponent = nullptr;

	int _weaponIndex = UNDEFINED_INDEX;

	int _nextWeaponIndex = UNDEFINED_INDEX;

	TArray<TFunction<bool(Item*)>> _itemProcessors;

	bool TryProcessItemAsGun(Item* item);

	bool TryProcessItemAsAmmo(Item* item);

	AmmoProvider* GetAmmoProvider(const FName ammoId);

	AmmoProvider* CreateAmmoProvider(const GunItem* gunItem);

	AmmoProvider* CreateAmmoProvider(const AmmoItem* ammoItem);

	AActor* CreateGun(const GunItem* gunItem);

	FDelegateHandle _equipWeaponRequestReadyHandle;

	FTimerHandle _equipWeaponTimerHandle;
	
	bool TryEquipNextUsableWeapon(const int indexModifier);

	bool IsChangingWeapon() const;
	
	int GetNextUsableWeaponIndex(const int indexModifier) const;
	
	void RequestEquipWeapon(const int weaponIndex);
	
	void EquipWeaponOnRequestReady();

	void EquipWeaponOnTimer();
	
	void EquipWeaponImmediate(const int weaponIndex);
	
	void AttachWeapon(AActor* weaponActor) const;

	bool CanAttachWeapon() const;

	void OnPartialInitialization();

	void CreateStartWeapon();

	void OnGunShotPerformed(const IGun* gun, const bool isAnyBulletDamaged);

	void OnOutOfAmmo();

	void StartWeaponFire() const;

	void StopWeaponFire() const;
};
