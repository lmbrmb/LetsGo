#pragma once

#include "MappingComponent.h"
#include "InputToFpMovementMapping.generated.h"

///<summary>
///Maps user input to UFirstPersonMovementComponent method calls
///</summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UInputToFpMovementMapping final : public UMappingComponent
{
	GENERATED_BODY()
	
protected:
	virtual void Map() override;
};
