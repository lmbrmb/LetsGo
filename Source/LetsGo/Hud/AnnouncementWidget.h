#pragma once

#include "Blueprint/UserWidget.h"
#include "LetsGo/Analytics/MedalType.h"
#include "LetsGo/Announcements/MedalAnnouncement.h"
#include "LetsGo/Announcements/FragAnnouncement.h"

#include "AnnouncementWidget.generated.h"

/// <summary>
/// Announcement widget
/// </summary>
UCLASS()
class LETSGO_API UAnnouncementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnMatchWarmUpAnnouncementRequest();
	
	void OnMatchStartAnnouncementRequest();

	void OnMatchEndAnnouncementRequest();
	
	void OnFragAnnouncementRequest(const FragAnnouncement* fragAnnouncement);

	void OnMedalAnnouncementRequest(const MedalAnnouncement* medalAnnouncement);

	void OnAllAnnouncementsDone();
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMatchWarmUp();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMatchStart();

	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMatchEnd(bool isLocalPlayerWonMatch);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceFrag(
		const FName& instigatorPlayerNickname,
		const FName& fraggedPlayerNickname,
		const bool isLocalPlayerInstigator,
		const bool isLocalPlayerFragged
	);

	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMedal(const FMedalType medalType);

	UFUNCTION(BlueprintImplementableEvent)
	void BpAllAnnouncementsDone();
};
