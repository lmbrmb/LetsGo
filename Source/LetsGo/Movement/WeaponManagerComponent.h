#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LetsGo/WeaponSystem/WeaponFactory.h"
#include "WeaponManagerComponent.generated.h"

UCLASS( ClassGroup=(_Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UWeaponManagerComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponManagerComponent();
	
protected:
	virtual void BeginPlay() override;

private:
	void MapPlayerInput(UInputComponent* playerInputComponent);

	void PrimaryFire();

	UFUNCTION(BlueprintCallable)
	void OnInventoryItemAdded(FName itemId);

	UFUNCTION(BlueprintCallable)
	void OnInventoryItemRemoved(FName itemId);

	WeaponFactory _weaponFactory;

	TArray<AWeaponBase*> _weapons;
};
