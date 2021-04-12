#include "MainMenuGameMode.h"

#include "LetsGo/DiContainers/MainMenuDiContainerFactory.h"
#include "LetsGo/Utils/AssertUtils.h"

TTypeContainer<ESPMode::Fast>* AMainMenuGameMode::GetDiContainer() const
{
	return _diContainer;
}

void AMainMenuGameMode::BeginPlay()
{
	AssertIsNotNull(_mainMenuWidgetBlueprint);

	auto const world = GetWorld();
	_mainMenuWidget = CreateWidget(world, _mainMenuWidgetBlueprint);
	_mainMenuWidget->AddToViewport(INT_MIN);
}

void AMainMenuGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	MainMenuDiContainerFactory<ESPMode::Fast> mainMenuDiContainerFactory;
	_diContainer = mainMenuDiContainerFactory.CreateContainer(this);
}
