#include "ProtagonistPawn.h"
#include "LetsGoGameModeBase.h"
#include "Movement/MovementComponentBase.h"

AProtagonistPawn::AProtagonistPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

AProtagonistPawn::~AProtagonistPawn()
{
}

UActorComponent* AProtagonistPawn::GetComponent(FName componentName) const
{
	if (componentName == "Movement") 
	{
		return FindComponentByClass<UMovementComponentBase>();
	}
	return nullptr;
}
