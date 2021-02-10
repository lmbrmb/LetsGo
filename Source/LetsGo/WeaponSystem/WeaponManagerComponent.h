#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBase.h"
#include "WeaponFactory.h"
#include "WeaponManagerComponent.generated.h"

class InventoryItem;
UCLASS( ClassGroup=(_Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UWeaponManagerComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponManagerComponent();

	void OnInventoryItemAdded(InventoryItem* item);

	void OnInventoryItemRemoved(InventoryItem* item);

	void StartFire();

	void StopFire();
	
	void Reload();

	void NextWeapon();

	void PreviousWeapon();

	void ChangeWeapon(float value);
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Custom)
		bool _equipWeaponOnPickup = true;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Custom)
	USceneComponent* _weaponPivot = nullptr;
	
	WeaponFactory _weaponFactory;
	
	TArray<AWeaponBase*> _weapons;

	void EquipWeapon(AWeaponBase* weapon);

	AWeaponBase* _currentWeapon;
};
