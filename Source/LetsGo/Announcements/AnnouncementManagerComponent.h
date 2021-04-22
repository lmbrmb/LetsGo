#pragma once

#include "Components/ActorComponent.h"
#include "IAnnouncement.h"
#include "IAnnouncementManager.h"
#include "Frag/FragAnnouncementFactory.h"
#include "LetsGo/Analytics/Medal.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "MatchEnd/MatchEndAnnouncementFactory.h"
#include "MatchStart/MatchStartAnnouncementFactory.h"
#include "MatchWarmUp/MatchWarmUpAnnouncementFactory.h"
#include "Medal/MedalAnnouncementFactory.h"

#include "AnnouncementManagerComponent.generated.h"

///<summary>
///Announcement manager component
///</summary>
UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UAnnouncementManagerComponent : public UActorComponent, public IAnnouncementManager
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

	TQueue<IAnnouncement*> _announcements;
	
	FragAnnouncementFactory* _fragAnnouncementFactory;

	MedalAnnouncementFactory* _medalAnnouncementFactory;

	MatchWarmUpAnnouncementFactory* _matchWarmUpAnnouncementFactory;

	MatchStartAnnouncementFactory* _matchStartAnnouncementFactory;

	MatchEndAnnouncementFactory* _matchEndAnnouncementFactory;

	const float UNDEFINED_TIME = -1;
	
	float _nextAnnouncementTime = UNDEFINED_TIME;

	FTimerHandle _announcementDoneTimerHandle;
	
	void AddAnnouncement(IAnnouncement* announcement);
	
	void CreateAnnouncementTask(const float delay);
	
	void AnnounceOnTimer();

	void AllAnnouncementsDoneOnTimer() const;
};
