#include "MatchPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/Input/InputConstant.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

void AMatchPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AssertIsNotNull(InputComponent);
	InputComponent->BindAction(InputConstant::ActionEscape, IE_Pressed, this, &AMatchPlayerController::OnEscape);
}

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

// ReSharper disable once CppMemberFunctionMayBeConst
void AMatchPlayerController::OnEscape()
{
	UGameplayStatics::OpenLevel(this, _mainMenuLevelName, true);
}

AAvatar* AMatchPlayerController::GetAvatar() const
{
	return _avatar;
}
