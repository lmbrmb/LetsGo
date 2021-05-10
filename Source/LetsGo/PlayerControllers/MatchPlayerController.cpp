#include "MatchPlayerController.h"

#include "LetsGo/Input/InputConstant.h"
#include "LetsGo/Utils/AssertUtils.h"

void AMatchPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto const world = GetWorld();
	auto const authGameMode = world->GetAuthGameMode();
	_matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(_matchGameMode);

	SetupInput();
}

void AMatchPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetMousePositionToCenter();
}

void AMatchPlayerController::SetupInput()
{
	SetShowMouseCursor(false);

	FInputModeGameOnly inputModeUIOnly;
	inputModeUIOnly.SetConsumeCaptureMouseDown(true);
	SetInputMode(inputModeUIOnly);

	AssertIsNotNull(InputComponent);
	InputComponent->BindAction(InputConstant::ActionEscape, IE_Pressed, this, &AMatchPlayerController::OnEscape);
}

void AMatchPlayerController::SetMousePositionToCenter()
{
	GetViewportSize(_viewPortX, _viewPortY);
	auto const mouseX = _viewPortX / 2;
	auto const mouseY = _viewPortY / 2;
	SetMouseLocation(mouseX, mouseY);
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
	_matchGameMode->ExitToMainMenu();
}

AAvatar* AMatchPlayerController::GetAvatar() const
{
	return _avatar;
}
