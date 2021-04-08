#pragma once

#include "ProjectGameModeBase.h"

#include "Blueprint/UserWidget.h"

#include "MainMenuGameMode.generated.h"

/// <summary>
/// Main menu game mode
/// </summary>
UCLASS()
class LETSGO_API AMainMenuGameMode : public AProjectGameModeBase
{
	GENERATED_BODY()

public:
	// IDiContainerProvider implementation
	virtual TTypeContainer<ESPMode::Fast>* GetDiContainer() const override;

protected:
	virtual void BeginPlay() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> _mainMenuWidgetBlueprint;

	UUserWidget* _mainMenuWidget = nullptr;

	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;
};
