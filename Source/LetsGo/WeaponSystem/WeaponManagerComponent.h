#pragma once

#include "Components/ActorComponent.h"
#include "WeaponBase.h"
#include "WeaponFactory.h"
#include "LetsGo/InventorySystem/InventoryItem.h"

#include "WeaponManagerComponent.generated.h"

const int UNDEFINED_INDEX = -1;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UWeaponManagerComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponManagerComponent();

	virtual void BeginPlay() override;
	
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
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Custom)
	bool _equipWeaponOnPickup = true;

	USceneComponent* _aimProvider;
	
	TArray<USceneComponent*> _weaponPivots;

	USceneComponent* _weaponPivot = nullptr;
	
	int _weaponPivotIndex = UNDEFINED_INDEX;
	
	WeaponFactory* _weaponFactory;
	
	TArray<AWeaponBase*> _weapons;

	void EquipWeapon(int weaponIndex);
	
	AWeaponBase* _weapon = nullptr;

	int _weaponIndex = UNDEFINED_INDEX;
};
