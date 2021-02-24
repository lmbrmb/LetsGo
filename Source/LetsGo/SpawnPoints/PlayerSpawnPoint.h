#pragma once

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
};
