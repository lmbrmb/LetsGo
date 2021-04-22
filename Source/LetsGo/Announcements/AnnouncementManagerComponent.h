#pragma once

#include "Components/ActorComponent.h"
#include "Announcement.h"
#include "MedalAnnouncement.h"
#include "FragAnnouncement.h"
#include "LetsGo/Analytics/Medal.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/GameModes/MatchGameMode.h"

#include "AnnouncementManagerComponent.generated.h"

DECLARE_EVENT(UAnnouncementManagerComponent, EMatchWarmUpAnnouncementRequest);

DECLARE_EVENT(UAnnouncementManagerComponent, EMatchStartAnnouncementRequest);

DECLARE_EVENT_OneParam(UAnnouncementManagerComponent, EMatchEndAnnouncementRequest, const int localPlayerPlace);

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

	void OnMatchWarmUp();
	
	void OnMatchStart();

	void OnMatchEnd();
	
	void OnMedalAchieved(const Medal& medal);

	void OnPlayerFragged(
		const PlayerId& instigatorPlayerId,
		const PlayerId& fraggedPlayerId
	);

	EMatchWarmUpAnnouncementRequest MatchWarmUpAnnouncementRequest;
	
	EMatchStartAnnouncementRequest MatchStartAnnouncementRequest;

	EMatchEndAnnouncementRequest MatchEndAnnouncementRequest;
	
	EFragAnnouncementRequest FragAnnouncementRequest;

	EMedalAnnouncementRequest MedalAnnouncementRequest;

	EAllAnnouncementsDone AllPlayerAnnouncementsDone;

	EAllAnnouncementsDone AllMatchAnnouncementsDone;
	
protected:
	virtual void BeginPlay() override;
	
private:
	PlayerId _playerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchWarmUpAnnouncementDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchStartAnnouncementDuration = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchEndAnnouncementDuration = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _firstPlayerAnnouncementDelay = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _playerAnnouncementDuration = 1.5f;

	AMatchGameMode* _matchGameMode = nullptr;

	TQueue<Announcement*> _playerAnnouncements;
	
	TArray<TFunction<bool(Announcement*)>> _announcementProcessors;

	const float UNDEFINED_TIME = -1;
	
	float _nextAnnouncementTime = UNDEFINED_TIME;

	FTimerHandle _announcementDoneTimerHandle;
	
	void AddPlayerAnnouncement(Announcement* announcement);
	
	void CreatePlayerAnnouncementTask(const float delay);

	void CreateAllMatchAnnouncementsDoneTask(const float delay);
	
	void PlayerAnnouncementOnTimer();

	void AllPlayerAnnouncementsDoneOnTimer() const;

	void AllMatchAnnouncementsDoneOnTimer() const;
	
	void ProcessAnnouncement(Announcement* announcement);
	
	bool TryProcessMedalAnnouncement(Announcement* announcement) const;

	bool TryProcessFragAnnouncement(Announcement* announcement) const;
};
