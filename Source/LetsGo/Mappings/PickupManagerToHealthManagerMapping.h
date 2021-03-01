#pragma once

#include "MappingComponent.h"

#include "PickupManagerToHealthManagerMapping.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UPickupManagerToHealthManagerMapping final : public UMappingComponent
{
	GENERATED_BODY()

public:	
	UPickupManagerToHealthManagerMapping() = default;

protected:
	virtual void Map() override;
};
