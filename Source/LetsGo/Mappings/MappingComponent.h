#pragma once

#include "Components/ActorComponent.h"
#include "MappingComponent.generated.h"

UCLASS(Abstract, ClassGroup=(Custom) )
class LETSGO_API UMappingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMappingComponent();

protected:
	virtual void BeginPlay() override;

	virtual void Map() {};

	/// <summary>
	/// [Template method]
	/// </summary>
	virtual bool ShouldDestroyAfterMapping() const;
};
