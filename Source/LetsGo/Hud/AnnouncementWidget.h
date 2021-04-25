#pragma once

#include "AnnouncementProcessors/IAnnouncementProcessor.h"
#include "Blueprint/UserWidget.h"
#include "LetsGo/Analytics/MedalType.h"
#include "LetsGo/Announcements/IAnnouncement.h"
#include "LetsGo/Announcements/IAnnouncer.h"

#include "AnnouncementWidget.generated.h"

/// <summary>
/// Announcement widget
/// </summary>
UCLASS()
class LETSGO_API UAnnouncementWidget : public UUserWidget, public IAnnouncer
{
	GENERATED_BODY()

public:
	// IAnnouncer implementation
	virtual void OnAnnouncementRequest(const IAnnouncement* announcement) override;
	
	// IAnnouncer implementation
	virtual void OnAllAnnouncementsDone() override;

	// IAnnouncer implementation
	virtual void AnnounceMatchWarmUp() override;

	// IAnnouncer implementation
	virtual void AnnounceMatchStart() override;

	// IAnnouncer implementation
	virtual void AnnounceMatchEnd(const int localPlayerPlace) override;

	// IAnnouncer implementation
	virtual void AnnounceFrag(
		const FName& instigatorPlayerNickname,
		const FName& fraggedPlayerNickname,
		const bool isLocalPlayerInstigator,
		const bool isLocalPlayerFragged,
		const int instigatorPlayerPlace,
		const int fraggedPlayerPlace
	) override;

	// IAnnouncer implementation
	virtual void AnnounceMedal(const FMedalType medalType) override;

	// IAnnouncer implementation
	virtual void AnnounceLead(const FLeadState leadState) override;

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMatchWarmUp();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMatchStart();

	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMatchEnd(const int localPlayerPlace);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceFrag(
		const FName& instigatorPlayerNickname,
		const FName& fraggedPlayerNickname,
		const bool isLocalPlayerInstigator,
		const bool isLocalPlayerFragged,
		const int instigatorPlayerPlace,
		const int fraggedPlayerPlace
	);

	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMedal(const FMedalType medalType);

	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceLead(const FLeadState leadState);

	UFUNCTION(BlueprintImplementableEvent)
	void BpAllAnnouncementsDone();

private:
	void ProcessAnnouncement(IAnnouncement* announcement);

	TArray<IAnnouncementProcessor*> _announcementProcessors;
};
