#pragma once

#include "MappingComponent.h"
#include "InputToTpMovementMapping.generated.h"

///<summary>
///Maps user input to UThirdPersonMovementComponent method calls
///</summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UInputToTpMovementMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;
};
