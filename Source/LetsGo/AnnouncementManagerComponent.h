#pragma once

#include "Analytics/Medal.h"
#include "Analytics/MedalType.h"
#include "Components/ActorComponent.h"

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

	void SetPlayerId(int32 playerId);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounceMedal(const FMedalType medalType);
	
	void OnMedalAchieved(const Medal& medal);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _announcementDelay = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _consequentAnnouncementDelay = 1.5f;

	TQueue<FMedalType> _medalsToAnnounce;

	int32 _playerId;
	
	void AnnounceMedalOnTimer();

	void CreateMedalAnnouncementTask(const float delay);
};
