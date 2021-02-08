#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LetsGo/InventorySystem/InventoryComponent.h"
#include "LetsGo/Movement/WeaponManagerComponent.h"
#include "InventoryToWeaponManagerMapping.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UInventoryToWeaponManagerMapping : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryToWeaponManagerMapping();

protected:
	virtual void BeginPlay() override;

private:
	UInventoryComponent* _inventoryComponent;

	UWeaponManagerComponent* _weaponManagerComponent;
};
