#pragma once

#include "GameFramework/Pawn.h"
#include "ProtagonistPawn.generated.h"

UCLASS()
class LETSGO_API AProtagonistPawn : public APawn
{
	GENERATED_BODY()

public:
	AProtagonistPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
