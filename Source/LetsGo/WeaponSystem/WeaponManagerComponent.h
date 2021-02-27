#pragma once

#include "Components/ActorComponent.h"
#include "WeaponBase.h"
#include "WeaponFactory.h"
#include "LetsGo/InventorySystem/AmmoItemFactory.h"
#include "LetsGo/InventorySystem/InventoryItem.h"

#include "WeaponManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UWeaponManagerComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponManagerComponent();
	
	void OnInventoryItemAdded(InventoryItem* item);

	void StartFire();

	void StopFire();
	
	void Reload();

	void NextWeapon();

	void PreviousWeapon();

	void ChangeWeapon(int indexModifier);

	void ChangeWeaponPivot();
	
	UFUNCTION(BlueprintCallable)
	void AddWeaponPivot(USceneComponent* weaponPivot);

	UFUNCTION(BlueprintCallable)
	void SetAimProvider(USceneComponent* aimProvider);

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
	
	WeaponFactory* _weaponFactory;
	
	TArray<AWeaponBase*> _weapons;

	AWeaponBase* _weapon = nullptr;

	int _weaponIndex = UNDEFINED_INDEX;
	
	void EquipWeapon(int weaponIndex);

	bool TryProcessItemAsWeapon(InventoryItem* item);
	
	bool TryProcessItemAsAmmo(InventoryItem* item);

	TArray<TFunction<bool(InventoryItem*)>> _itemProcessors;
};
