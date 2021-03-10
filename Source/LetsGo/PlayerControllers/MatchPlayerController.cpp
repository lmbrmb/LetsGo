#include "MatchPlayerController.h"

void AMatchPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	_avatar = static_cast<AAvatar*>(InPawn);
}

AAvatar* AMatchPlayerController::GetAvatar() const
{
	return _avatar;
}
