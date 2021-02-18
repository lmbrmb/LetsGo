#pragma once

#include "MappingComponent.h"
#include "InventoryToHealthMapping.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UInventoryToHealthMapping final : public UMappingComponent
{
	GENERATED_BODY()

public:	
	UInventoryToHealthMapping();

protected:
	virtual void Map() override;
};
