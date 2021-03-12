#include "MatchSpectatorPawn.h"

void AMatchSpectatorPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Possessed.Broadcast();
}
