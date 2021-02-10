#include "ProtagonistPawn.h"

AProtagonistPawn::AProtagonistPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	_healthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
}

AProtagonistPawn::~AProtagonistPawn()
{
}
