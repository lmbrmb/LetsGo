#pragma once

#include "LetsGo/PickupItems/PickupItemFactory.h"
#include "PickupSpawnPoint.generated.h"

///<summary>
///Pickup spawn point. Spawns pickup on startup
///</summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API APickupSpawnPoint : public AActor
{
	GENERATED_BODY()

public:	
	APickupSpawnPoint();

protected:
	virtual void BeginPlay() override;

private:
	PickupItemFactory* _pickupItemFactory = nullptr;

	void SpawnPickup();

	UPROPERTY(BlueprintReadOnly, Category = Custom, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _root = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _spawnPivot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _id = "";
};
