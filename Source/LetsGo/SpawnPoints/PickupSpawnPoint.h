#pragma once

#include "SpawnPointType.h"
#include "LetsGo/Pickups/PickupItem.h"
#include "LetsGo/AssetFactories/PickupItemFactory.h"

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

	float GetPickupSpawnTime() const;
	
protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

private:
	const float UNDEFINED_TIME = -1;

	UPROPERTY(BlueprintReadOnly, Category = Custom, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _root = nullptr;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _spawnPivot = nullptr;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _pickupRespawnInterval = 10;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _id = "";

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FSpawnPointType _spawnPointType;

	APickupItem* _pickupItem;
	
	PickupItemFactory* _pickupItemFactory;

	float _pickupSpawnTime = 0;

	FDelegateHandle _delegateHandleOnPickupTaken;

	FTimerHandle _pickupRespawnTimerHandle;

	void CreatePickup();

	void SpawnPickup();

	void OnPickupTaken(APickupItem* pickupItem);

	void RespawnPickupOnTimer();
};
