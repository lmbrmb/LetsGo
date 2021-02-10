#pragma once

#include "MappingComponent.h"
#include "InputToWeaponManagerMapping.generated.h"

///<summary>
///Maps user input to UWeaponManagerComponent methods
///</summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UInputToWeaponManagerMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;
};
