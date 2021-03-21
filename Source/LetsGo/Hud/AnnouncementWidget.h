#pragma once

#include "Blueprint/UserWidget.h"
#include "LetsGo/Analytics/MedalType.h"

#include "AnnouncementWidget.generated.h"

/// <summary>
/// Announcement widget
/// </summary>
UCLASS()
class LETSGO_API UAnnouncementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnMessageAnnouncementRequest(const FString& message);

	void OnMedalAnnouncementRequest(const FMedalType medalType);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMessage(const FString& message);

	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMedal(const FMedalType medalType);
};
