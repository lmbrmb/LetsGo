#include "ProtagonistPlayerController.h"
#include "Kismet/KismetStringLibrary.h"
#include "Logs/DevLogger.h"

void AProtagonistPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	_protagonistPawn = static_cast<AProtagonistPawn*>(InPawn);
}

void AProtagonistPlayerController::DebugDisplayPawnInfo() const
{
	const auto isPawnAssigned = GetPawn() != nullptr;
	const auto isProtagonistPawnAssigned = _protagonistPawn != nullptr;
	
	const auto pawnAssignedMsg = "Pawn is assigned: " + UKismetStringLibrary::Conv_BoolToString(isPawnAssigned);
	const auto protagonistPawnAssignedMsg = "Protagonist pawn is assigned: " + UKismetStringLibrary::Conv_BoolToString(isProtagonistPawnAssigned);

	DevLogger::GetLoggingChannel()->Log(pawnAssignedMsg + " / " + protagonistPawnAssignedMsg);
}
