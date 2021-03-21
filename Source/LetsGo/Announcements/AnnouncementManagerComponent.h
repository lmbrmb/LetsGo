#pragma once

#include "Components/ActorComponent.h"
#include "Announcement.h"
#include "LetsGo/Analytics/Medal.h"
#include "LetsGo/Data/PlayerId.h"

#include "AnnouncementManagerComponent.generated.h"

DECLARE_EVENT_OneParam(UAnnouncementManagerComponent, EMessageAnnouncementRequest, const FString& message);

DECLARE_EVENT_OneParam(UAnnouncementManagerComponent, EMedalAnnouncementRequest, const FMedalType message);

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
	
	void OnMedalAchieved(const Medal& medal);

	void OnPlayerFragged(
		const PlayerId& instigatorPlayerId,
		const PlayerId& fraggedPlayerId,
		const FName& instigatorPlayerNickname,
		const FName& fraggedPlayerNickname
	);

	EMessageAnnouncementRequest MessageAnnouncementRequest;

	EMedalAnnouncementRequest MedalAnnouncementRequest;
	
protected:
	virtual void BeginPlay() override;
	
private:
	PlayerId _playerId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _announcementDelay = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _consequentAnnouncementDelay = 1.5f;

	TQueue<Announcement*> _announcements;

	TArray<TFunction<bool(Announcement*)>> _announcementProcessors;

	void CreateAnnouncementTask(const float delay);

	void AnnounceOnTimer();
	
	void ProcessAnnouncement(Announcement* announcement) const;
	
	bool TryProcessMedalAnnouncement(Announcement* announcement) const;

	bool TryProcessMessageAnnouncement(Announcement* announcement) const;
};
