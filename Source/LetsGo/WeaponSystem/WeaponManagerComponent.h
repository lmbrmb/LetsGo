#pragma once

#include "AmmoProvider.h"
#include "Components/ActorComponent.h"
#include "WeaponBase.h"
#include "Gun.h"
#include "GunFactory.h"
#include "LetsGo/Items/AmmoItem.h"
#include "LetsGo/Items/AmmoItemFactory.h"
#include "LetsGo/Items/GunItem.h"
#include "LetsGo/Items/IItemProcessor.h"
#include "LetsGo/Items/Item.h"
#include "WeaponManagerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	virtual bool TryProcessItem(Item* item) override;
	
protected:
	virtual void BeginPlay() override;

private:
	const int UNDEFINED_INDEX = -1;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Custom)
	bool _equipWeaponOnPickup = true;

	USceneComponent* _aimProvider;

	TArray<USceneComponent*> _weaponPivots;

	USceneComponent* _weaponPivot = nullptr;

	int _weaponPivotIndex = UNDEFINED_INDEX;

	GunFactory* _gunFactory;

	AmmoItemFactory* _ammoItemFactory;

	TArray<AWeaponBase*> _weapons;

	/// <summary>
	/// Gun Id / Ammo provider
	/// </summary>
	TMap<FName, AmmoProvider*> _ammoProviders;

	AWeaponBase* _weapon = nullptr;

	int _weaponIndex = UNDEFINED_INDEX;

	void EquipWeapon(int weaponIndex);

	TArray<TFunction<bool(Item*)>> _itemProcessors;

	bool TryProcessItemAsGun(Item* item);
	
	bool TryProcessItemAsAmmo(Item* item);

	AmmoProvider* GetAmmoProvider(const FName ammoId);
	
	AmmoProvider* CreateAmmoProvider(const GunItem* gunItem);

	AmmoProvider* CreateAmmoProvider(const AmmoItem* ammoItem);

	AGun* CreateGun(const GunItem* gunItem);
};
