#include "MainMenuGameMode.h"

#include "LetsGo/Utils/AssertUtils.h"


void AMainMenuGameMode::BeginPlay()
{
	AssertIsNotNull(_mainMenuWidgetBlueprint);

	auto const world = GetWorld();
	_mainMenuWidget = CreateWidget(world, _mainMenuWidgetBlueprint);
	_mainMenuWidget->AddToViewport(INT_MIN);
}
