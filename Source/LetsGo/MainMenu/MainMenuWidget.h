#pragma once

#include "Blueprint/UserWidget.h"

#include "LetsGo/GameModes/MainMenuGameMode.h"

#include "MainMenuWidget.generated.h"

/// <summary>
/// Main menu widget
/// </summary>
UCLASS()
class LETSGO_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	UPlayerSettings* GetPlayerSettings() const;

	UFUNCTION(BlueprintCallable)
	UMatchSettings* GetMatchSettings() const;

	UFUNCTION(BlueprintCallable)
	void SavePlayerSettings() const;

	UFUNCTION(BlueprintCallable)
	void SaveMatchSettings() const;

private:
	AMainMenuGameMode* _mainMenuGameMode = nullptr;
};
