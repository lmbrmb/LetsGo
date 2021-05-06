#pragma once

#include "Components/ActorComponent.h"
#include "IGun.h"
#include "LetsGo/AssetFactories/GunFactory.h"
#include "LetsGo/AimProviders/IAimProvider.h"
#include "LetsGo/AmmoProviders/IAmmoProvider.h"
#include "LetsGo/AmmoProviders/AmmoProviderFactory.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Items/AmmoItem.h"
#include "LetsGo/Items/AmmoItemFactory.h"
#include "LetsGo/Items/GunItem.h"
#include "LetsGo/Items/GunItemFactory.h"
#include "LetsGo/Items/IItemProcessor.h"
#include "LetsGo/Items/Item.h"

#include "WeaponManagerComponent.generated.h"

DECLARE_EVENT_ThreeParams(
UWeaponManagerComponent,
EShotPerformed_UWeaponManagerComponent,
const PlayerId& playerId,
const WeaponType& weaponType,
const bool isAnyBulletDamaged
);

DECLARE_EVENT(UWeaponManagerComponent, EWeaponChanged_UWeaponManagerComponent);

DECLARE_EVENT(UWeaponManagerComponent, EShotRequested_UWeaponManagerComponent);

DECLARE_EVENT(UWeaponManagerComponent, EInitialized_UWeaponManagerComponent);

DECLARE_EVENT_OneParam(
UWeaponManagerComponent,
EWeaponPivotChanged_UWeaponManagerComponent,
const int pivotIndex
);

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

	void ChangeWeapon(const int indexModifier);

	void ChangeWeapon(const FName& weaponId);

	void ChangeWeaponPivot();

	void SetWeaponPivot(const int weaponPivotIndex);

	IAimProvider* GetAimProvider() const;

	void DisableWeapon();

	void SetAimProvider(IAimProvider* aimProvider);

	UFUNCTION(BlueprintCallable)
	void InitializeWeaponPivots(const TArray<USceneComponent*>& weaponPivots);

	UFUNCTION(BlueprintCallable)
	int GetWeaponsCount() const;

	UFUNCTION(BlueprintCallable)
	void SetOwnerSkeletalMeshComponent(USkeletalMeshComponent* ownerSkeletalMeshComponent);

	UFUNCTION(BlueprintCallable)
	FName GetCurrentWeaponId() const;

	TArray<IWeapon*>& GetWeapons();

	TArray<IGun*>& GetGuns();

	void SetPlayerId(const PlayerId& playerId);

	virtual bool TryProcessItem(Item* item) override;

	IWeapon* GetCurrentWeapon() const;

	IGun* GetCurrentGun() const;

	bool IsInitialized() const;

	bool IsChangingWeapon() const;

	EInitialized_UWeaponManagerComponent Initialized;

	EShotPerformed_UWeaponManagerComponent ShotPerformed;

	EWeaponChanged_UWeaponManagerComponent WeaponEquipped;

	EWeaponChanged_UWeaponManagerComponent WeaponHolstered;

	EShotRequested_UWeaponManagerComponent ShotRequested;

	EWeaponPivotChanged_UWeaponManagerComponent WeaponPivotChanged;

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
	TMap<FName, IAmmoProvider*> _ammoProviders;

	USkeletalMeshComponent* _ownerSkeletalMeshComponent = nullptr;

	int _weaponIndex = UNDEFINED_INDEX;

	int _nextWeaponIndex = UNDEFINED_INDEX;

	IWeapon* _requestReadyWeapon = nullptr;

	TArray<TFunction<bool(Item*)>> _itemProcessors;

	bool TryProcessItemAsGun(Item* item);

	bool TryProcessItemAsAmmo(Item* item);

	IAmmoProvider* GetAmmoProvider(const FName ammoId);

	IAmmoProvider* CreateAmmoProvider(const GunItem* gunItem);

	IAmmoProvider* CreateAmmoProvider(const AmmoItem* ammoItem);

	AActor* CreateGun(const GunItem* gunItem);

	FDelegateHandle _equipWeaponRequestReadyHandle;

	FTimerHandle _equipWeaponTimerHandle;

	AmmoProviderFactory* _ammoProviderFactory;

	void EquipNextUsableWeapon(const int indexModifier);

	int GetNextUsableWeaponIndex(const int indexModifier) const;

	void RequestEquipWeapon(const int weaponIndex);

	void EquipWeaponOnRequestReady();

	void SubscribeWeaponRequestReady(IWeapon* weapon);

	void UnsubscribeWeaponRequestReady();

	void EquipWeaponOnTimer();

	void EquipWeaponImmediate(const int weaponIndex);

	void AttachWeapon(AActor* weaponActor) const;

	bool CanAttachWeapon() const;

	void OnPartialInitialization();

	void CreateStartWeapon();

	void OnGunShotPerformed(const IGun* gun, const bool isAnyBulletDamaged);

	void OnGunShotRequested(const USceneComponent* firePivot) const;

	void OnOutOfAmmo();

	int GetWeaponIndex(const FName& weaponId);

	void StartWeaponFire() const;

	void StopWeaponFire() const;
};
