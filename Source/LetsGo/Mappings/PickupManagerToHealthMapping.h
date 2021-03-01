#pragma once

#include "MappingComponent.h"

#include "PickupManagerToHealthMapping.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UPickupManagerToHealthMapping final : public UMappingComponent
{
	GENERATED_BODY()

public:	
	UPickupManagerToHealthMapping() = default;

protected:
	virtual void Map() override;
};
