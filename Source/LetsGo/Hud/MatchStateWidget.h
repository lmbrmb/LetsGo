#pragma once

#include "Blueprint/UserWidget.h"
#include "LetsGo/GameModes/MatchGameMode.h"

#include "MatchStateWidget.generated.h"

/// <summary>
/// Match state widget. Shows match time and state
/// </summary>
UCLASS()
class LETSGO_API UMatchStateWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	void OnMatchWarmUp();

	void OnMatchStart();

	void OnMatchEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnMatchWarmUp();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnMatchStart();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnMatchEnd();

	UFUNCTION(BlueprintCallable)
	float GetMatchTime() const;

private:
	AMatchGameMode* _matchGameMode = nullptr;
};
