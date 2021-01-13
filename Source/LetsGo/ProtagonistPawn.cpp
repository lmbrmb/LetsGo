#include "ProtagonistPawn.h"

AProtagonistPawn::AProtagonistPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AProtagonistPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AProtagonistPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProtagonistPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
