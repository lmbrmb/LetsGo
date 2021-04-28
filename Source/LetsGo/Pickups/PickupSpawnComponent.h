#pragma once

#include "Components/ActorComponent.h"
#include "LetsGo/AssetFactories/PickupItemFactory.h"

#include "PickupSpawnComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UPickupSpawnComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickupSpawnComponent();

	UFUNCTION(BlueprintCallable)
	void SpawnPickup(const FName& id, const FVector& location);

	PickupItemFactory* _pickupItemFactory;

protected:
	virtual void BeginPlay() override;
};
