#include "MainMenuWidget.h"

#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	_mainMenuGameMode = Cast<AMainMenuGameMode>(authGameMode);
	AssertIsNotNull(_mainMenuGameMode);
}

UPlayerSettings* UMainMenuWidget::GetPlayerSettings() const
{
	return _mainMenuGameMode->GetPlayerSettings();
}

UMatchSettings* UMainMenuWidget::GetMatchSettings() const
{
	return _mainMenuGameMode->GetMatchSettings();
}

void UMainMenuWidget::SavePlayerSettings() const
{
	_mainMenuGameMode->SavePlayerSettings();
}

void UMainMenuWidget::SaveMatchSettings() const
{
	_mainMenuGameMode->SaveMatchSettings();
}
