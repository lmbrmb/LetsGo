#pragma once

#include "LetsGo/PickupItems/PickupItem.h"
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
	const float UNDEFINED_TIME = -1;
	
	UBlueprint* _pickupItemBlueprint = nullptr;

	void SpawnPickup();

	void OnPickupTaken(APickupItem* pickupItem);

	UFUNCTION()
	void RespawnPickupOnTimer();
	
	UPROPERTY(BlueprintReadOnly, Category = Custom, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _root = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _spawnPivot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _pickupRespawnTime = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _id = "";

	FDelegateHandle _delegateHandleOnPickupTaken;
	
	FTimerHandle _timerHandle;
};
