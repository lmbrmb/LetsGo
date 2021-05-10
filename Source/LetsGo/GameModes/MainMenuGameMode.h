#pragma once

#include "ProjectGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "LetsGo/Settings/MatchSettingsManager.h"
#include "LetsGo/Settings/PlayerSettingsManager.h"

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

	~AMainMenuGameMode();

	UPlayerSettings* GetPlayerSettings() const;

	UMatchSettings* GetMatchSettings() const;

	void SavePlayerSettings() const;

	void SaveMatchSettings() const;

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> _mainMenuWidgetBlueprint;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FString _playerSettingsSlotName = "PlayerSettings";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FString _matchSettingsSlotName = "MatchSettings";

	UPROPERTY()
	UUserWidget* _mainMenuWidget = nullptr;

	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;

	MatchSettingsManager* _matchSettingsManager = nullptr;

	PlayerSettingsManager* _playerSettingsManager = nullptr;
};
