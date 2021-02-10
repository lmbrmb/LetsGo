#pragma once

#include "MappingComponent.h"
#include "InventoryToWeaponManagerMapping.generated.h"

///<summary>
///Maps UInventoryComponent and WeaponManagerComponent interaction (via event callbacks).
///</summary>
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LETSGO_API UInventoryToWeaponManagerMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;
};
