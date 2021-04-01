#pragma once

#include "SpawnPointType.h"

#include "GameFramework/Actor.h"

#include "PlayerSpawnPoint.generated.h"

UCLASS()
class LETSGO_API APlayerSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerSpawnPoint();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FSpawnPointType _spawnPointType;
};
