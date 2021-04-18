#pragma once

#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/// <summary>
/// Main menu player controller
/// </summary>
UCLASS()
class LETSGO_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void SetupInput();

	void OnEscape();
};
