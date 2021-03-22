#pragma once

#include "Components/ActorComponent.h"
#include "Announcement.h"
#include "MedalAnnouncement.h"
#include "FragAnnouncement.h"
#include "LetsGo/Analytics/Medal.h"
#include "LetsGo/Data/PlayerId.h"

#include "AnnouncementManagerComponent.generated.h"

DECLARE_EVENT(UAnnouncementManagerComponent, EMatchWarmUpAnnouncementRequest);

DECLARE_EVENT(UAnnouncementManagerComponent, EMatchStartAnnouncementRequest);

DECLARE_EVENT(UAnnouncementManagerComponent, EMatchEndAnnouncementRequest);

DECLARE_EVENT_OneParam(UAnnouncementManagerComponent, EFragAnnouncementRequest, const FragAnnouncement* fragAnnouncement);

DECLARE_EVENT_OneParam(UAnnouncementManagerComponent, EMedalAnnouncementRequest, const MedalAnnouncement* medalAnnouncement);

DECLARE_EVENT(UAnnouncementManagerComponent, EAllAnnouncementsDone);

///<summary>
///Announcement manager component
///</summary>
UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UAnnouncementManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UAnnouncementManagerComponent();

	void SetPlayerId(const PlayerId& playerId);

	void OnMatchWarmUp() const;
	
	void OnMatchStart() const;

	void OnMatchEnd() const;
	
	void OnMedalAchieved(const Medal& medal);

	void OnPlayerFragged(
		const PlayerId& instigatorPlayerId,
		const PlayerId& fraggedPlayerId,
		const FName& instigatorPlayerNickname,
		const FName& fraggedPlayerNickname
	);

	EMatchWarmUpAnnouncementRequest MatchWarmUpAnnouncementRequest;
	
	EMatchStartAnnouncementRequest MatchStartAnnouncementRequest;

	EMatchEndAnnouncementRequest MatchEndAnnouncementRequest;
	
	EFragAnnouncementRequest FragAnnouncementRequest;

	EMedalAnnouncementRequest MedalAnnouncementRequest;

	EAllAnnouncementsDone AllAnnouncementsDone;
	
protected:
	virtual void BeginPlay() override;
	
private:
	PlayerId _playerId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _firstAnnouncementDelay = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _announcementDuration = 1.5f;

	TQueue<Announcement*> _announcements;

	TArray<TFunction<bool(Announcement*)>> _announcementProcessors;

	const float UNDEFINED_TIME = -1;
	
	float _nextAnnouncementTime = UNDEFINED_TIME;

	FTimerHandle _announcementDoneTimerHandle;
	
	void AddAnnouncement(Announcement* announcement);
	
	void CreateAnnouncementTask(const float delay);
	
	void AnnounceOnTimer();

	void AllAnnouncementsDoneOnTimer() const;
	
	void ProcessAnnouncement(Announcement* announcement);
	
	bool TryProcessMedalAnnouncement(Announcement* announcement) const;

	bool TryProcessFragAnnouncement(Announcement* announcement) const;
};
