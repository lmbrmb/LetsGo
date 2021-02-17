#pragma once

#include "Components/ActorComponent.h"
#include "WeaponBase.h"
#include "WeaponFactory.h"
#include "WeaponManagerComponent.generated.h"

const int UNDEFINED_WEAPON_INDEX = -1;

class InventoryItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UWeaponManagerComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponManagerComponent();

	virtual void BeginPlay() override;
	
	void OnInventoryItemAdded(InventoryItem* item);

	void OnInventoryItemRemoved(InventoryItem* item);

	void StartFire();

	void StopFire();
	
	void Reload();

	void NextWeapon();

	void PreviousWeapon();

	void ChangeWeapon(int indexModifier);
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponPivot(USceneComponent* weaponPivot);

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Custom)
	bool _equipWeaponOnPickup = true;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Custom)
	USceneComponent* _weaponPivot = nullptr;

	WeaponFactory* _weaponFactory;
	
	TArray<AWeaponBase*> _weapons;

	void EquipWeapon(int weaponIndex);
	
	AWeaponBase* _currentWeapon = nullptr;

	int _currentWeaponIndex = UNDEFINED_WEAPON_INDEX;
};
