#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LetsGo/WeaponSystem/WeaponBase.h"
#include "LetsGo/WeaponSystem/WeaponFactory.h"
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
	
protected:
	virtual void BeginPlay() override;

private:
	void MapPlayerInput(UInputComponent* playerInputComponent);

	void PrimaryFire();

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Custom)
		bool _equipWeaponOnPickup = true;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Custom)
	USceneComponent* _weaponPivot = nullptr;
	
	WeaponFactory _weaponFactory;
	
	TArray<AWeaponBase*> _weapons;

	void EquipWeapon(AWeaponBase* weapon);

	AWeaponBase* _currentWeapon;
};
