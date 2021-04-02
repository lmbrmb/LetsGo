#pragma once

#include "Blueprint/UserWidget.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/GameModes/MatchGameMode.h"

#include "FragCountWidget.generated.h"

/// <summary>
/// Frag count widget
/// </summary>
UCLASS()
class LETSGO_API UFragCountWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnFragCountChanged(const int fragCount);
	
private:
	AMatchGameMode* _matchGameMode;

	PlayerId _localPlayerId;
	
	void OnPlayerFragged(
		const PlayerId& instigatorPlayerId,
		const PlayerId& fraggedPlayerId,
		const FName& instigatorPlayerNickname,
		const FName& fraggedPlayerNickname
	);
};
