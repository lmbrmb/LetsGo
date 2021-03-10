#pragma once

#include "Analytics/MatchHighlight.h"
#include "Components/ActorComponent.h"

#include "AnnouncementManagerComponent.generated.h"

///<summary>
///Announcement manager component
///</summary>
UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UAnnouncementManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:	
	UAnnouncementManagerComponent();

	UFUNCTION(BlueprintImplementableEvent)
	void BpAnnounce(const FMatchHighlight matchHighlight);
	
	void OnMatchHighlight(
		const FGuid& playerId,
		const FMatchHighlight matchHighlight
	);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _announcementDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _consequentAnnouncementDelay = 2.0f;
	
	TQueue<FMatchHighlight> _matchHighlights;

private:
	void AnnounceOnTimer();

	void CreateAnnouncementTask(const float delay);
};
