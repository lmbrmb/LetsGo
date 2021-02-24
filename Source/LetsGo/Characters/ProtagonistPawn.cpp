#include "ProtagonistPawn.h"

AProtagonistPawn::AProtagonistPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	_healthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
}

AProtagonistPawn::~AProtagonistPawn()
{
}
