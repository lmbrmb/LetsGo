#include "MainMenuGameMode.h"

#include "LetsGo/DiContainers/MainMenuDiContainerFactory.h"
#include "LetsGo/Settings/SettingsManagerFactory.h"
#include "LetsGo/Utils/AssertUtils.h"

TTypeContainer<ESPMode::Fast>* AMainMenuGameMode::GetDiContainer() const
{
	return _diContainer;
}

AMainMenuGameMode::~AMainMenuGameMode()
{
	delete _diContainer;
}

void AMainMenuGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	MainMenuDiContainerFactory<ESPMode::Fast> mainMenuDiContainerFactory;
	_diContainer = mainMenuDiContainerFactory.CreateContainer(this);

	auto const settingsManagerFactoryRef = GetDiContainer()->GetInstance<SettingsManagerFactory>();
	auto const settingsManagerFactory = &settingsManagerFactoryRef.Get();

	_playerSettingsManager = settingsManagerFactory->Create<PlayerSettingsManager>(_playerSettingsSlotName, true);
	_matchSettingsManager = settingsManagerFactory->Create<MatchSettingsManager>(_matchSettingsSlotName, true);
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	AssertIsNotNull(_mainMenuWidgetBlueprint);

	auto const world = GetWorld();
	_mainMenuWidget = CreateWidget(world, _mainMenuWidgetBlueprint);
	_mainMenuWidget->AddToViewport(INT_MIN);
}

UPlayerSettings* AMainMenuGameMode::GetPlayerSettings() const
{
	return _playerSettingsManager->GetSettings();
}

UMatchSettings* AMainMenuGameMode::GetMatchSettings() const
{
	return _matchSettingsManager->GetSettings();
}

void AMainMenuGameMode::SavePlayerSettings() const
{
	_playerSettingsManager->Save();
}

void AMainMenuGameMode::SaveMatchSettings() const
{
	_matchSettingsManager->Save();
}
