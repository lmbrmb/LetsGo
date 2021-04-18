#include "MainMenuPlayerController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "LetsGo/Input/InputConstant.h"
#include "LetsGo/Utils/AssertUtils.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupInput();
}

void AMainMenuPlayerController::SetupInput()
{
	SetShowMouseCursor(true);

	FInputModeGameAndUI inputModeGameAndUi;
	inputModeGameAndUi.SetHideCursorDuringCapture(false);
	SetInputMode(inputModeGameAndUi);
	
	AssertIsNotNull(InputComponent);
	InputComponent->BindAction(InputConstant::ActionEscape, IE_Pressed, this, &AMainMenuPlayerController::OnEscape);
}

void AMainMenuPlayerController::OnEscape()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}
