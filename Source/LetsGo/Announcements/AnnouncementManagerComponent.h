#pragma once

#include "Components/ActorComponent.h"

#include "LetsGo/Analytics/Medal.h"
#include "LetsGo/Data/PlayerId.h"

#include "AnnouncementManagerComponent.generated.h"

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

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BpOnAnnounceMedal(const FMedalType medalType);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _announcementDelay = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _consequentAnnouncementDelay = 1.5f;

	TQueue<FMedalType> _medalsToAnnounce;

	PlayerId _playerId;
	
	void AnnounceMedalOnTimer();

	void CreateMedalAnnouncementTask(const float delay);
};
