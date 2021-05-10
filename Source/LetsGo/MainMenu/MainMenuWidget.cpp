#include "MainMenuWidget.h"

#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	_mainMenuGameMode = Cast<AMainMenuGameMode>(authGameMode);
	AssertIsNotNull(_mainMenuGameMode);
}

UPlayerSettings* UMainMenuWidget::GetPlayerSettings() const
{
	AssertIsNotNull(_mainMenuGameMode, nullptr);
	return _mainMenuGameMode->GetPlayerSettings();
}

UMatchSettings* UMainMenuWidget::GetMatchSettings() const
{
	AssertIsNotNull(_mainMenuGameMode, nullptr);
	return _mainMenuGameMode->GetMatchSettings();
}

void UMainMenuWidget::SavePlayerSettings() const
{
	AssertIsNotNull(_mainMenuGameMode);
	_mainMenuGameMode->SavePlayerSettings();
}

void UMainMenuWidget::SaveMatchSettings() const
{
	AssertIsNotNull(_mainMenuGameMode);
	_mainMenuGameMode->SaveMatchSettings();
}
