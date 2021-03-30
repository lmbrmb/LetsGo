#pragma once

#include "GameFramework/GameModeBase.h"

#include "Blueprint/UserWidget.h"

#include "MainMenuGameMode.generated.h"

/// <summary>
/// Main menu game mode
/// </summary>
UCLASS()
class LETSGO_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> _mainMenuWidgetBlueprint;

	UUserWidget* _mainMenuWidget = nullptr;
};
