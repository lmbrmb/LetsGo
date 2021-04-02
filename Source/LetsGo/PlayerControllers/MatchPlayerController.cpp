#include "MatchPlayerController.h"

#include "LetsGo/Input/InputConstant.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

void AMatchPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	_avatar = static_cast<AAvatar*>(InPawn);
	AvatarChanged.Broadcast(_avatar);
	AssertIsNotNull(_avatar);
}

void AMatchPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	_avatar = nullptr;
	AvatarChanged.Broadcast(nullptr);
}

AAvatar* AMatchPlayerController::GetAvatar() const
{
	return _avatar;
}
