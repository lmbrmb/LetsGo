#include "ProtagonistPawn.h"
#include "LetsGoGameModeBase.h"
#include "Logs/DevLogger.h"
#include "PawnControls/PawnControllerMappingFactory.h"

AProtagonistPawn::AProtagonistPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	RootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCapsuleComponent"));
	SetRootComponent(RootCapsuleComponent);
}

AProtagonistPawn::~AProtagonistPawn()
{
	if (_pawnControllerMapping)
	{
		delete _pawnControllerMapping;
	}
}

void AProtagonistPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (_pawnControllerMapping)
	{
		_pawnControllerMapping->Unmap();
		delete _pawnControllerMapping;
	}
	
	if(!PlayerInputComponent)
	{
		DevLogger::GetLoggingChannel()->Log("No player input component", LogSeverity::Error);
		return;
	}
	
	// TODO: inject factory
	PawnControllerMappingFactory controllerMappingFactory;
	_pawnControllerMapping = controllerMappingFactory.Create(ControlScheme);
	_pawnControllerMapping->Map(PlayerInputComponent, this);
}
