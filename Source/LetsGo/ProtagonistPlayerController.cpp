#include "ProtagonistPlayerController.h"
#include "LetsGoGameModeBase.h"
#include "Kismet/KismetStringLibrary.h"

void AProtagonistPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	_protagonistPawn = static_cast<AProtagonistPawn*>(InPawn);
	DebugDisplayPawnInfo();
}

void AProtagonistPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

LoggingChannel* AProtagonistPlayerController::GetLoggingChannel() const
{
	const auto gameMode = GetWorld()->GetAuthGameMode();
	const auto gameModeBase = static_cast<ALetsGoGameModeBase*>(gameMode);
	return gameModeBase
		? gameModeBase->GetLoggingChannel()
		: nullptr;
}

void AProtagonistPlayerController::DebugDisplayPawnInfo() const
{
	const auto isPawnAssigned = GetPawn() != nullptr;
	const auto isProtagonistPawnAssigned = _protagonistPawn != nullptr;
	
	const auto pawnAssignedMsg = "Pawn is assigned: " + UKismetStringLibrary::Conv_BoolToString(isPawnAssigned);
	const auto protagonistPawnAssignedMsg = "Protagonist pawn is assigned: " + UKismetStringLibrary::Conv_BoolToString(isProtagonistPawnAssigned);
	
	GetLoggingChannel()->Log(pawnAssignedMsg + " / " + protagonistPawnAssignedMsg);
}
