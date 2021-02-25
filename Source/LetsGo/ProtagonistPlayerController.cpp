#include "ProtagonistPlayerController.h"

void AProtagonistPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	_avatar = static_cast<AAvatar*>(InPawn);
}

AAvatar* AProtagonistPlayerController::GetAvatar()
{
	return _avatar;
}
