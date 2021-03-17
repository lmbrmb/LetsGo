#include "AnnouncementManagerComponent.h"
#include "GameFramework/GameModeBase.h"

UAnnouncementManagerComponent::UAnnouncementManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAnnouncementManagerComponent::SetPlayerId(const PlayerId& playerId)
{
	_playerId = playerId;
}

void UAnnouncementManagerComponent::OnMedalAchieved(const Medal& medal)
{
	if(medal.GetPlayerId() != _playerId)
	{
		return;
	}
	
	auto const queueWasEmpty = _medalsToAnnounce.IsEmpty();
	_medalsToAnnounce.Enqueue(medal.GetMedalType());
	
	if (!queueWasEmpty)
	{
		return;
	}

	CreateMedalAnnouncementTask(_announcementDelay);
}

void UAnnouncementManagerComponent::CreateMedalAnnouncementTask(const float delay)
{
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	FTimerHandle announceTimerHandle;
	authGameMode->GetWorldTimerManager().SetTimer(
		announceTimerHandle, 
		this, 
		&UAnnouncementManagerComponent::AnnounceMedalOnTimer, 
		delay, 
		false
	);
}

void UAnnouncementManagerComponent::AnnounceMedalOnTimer()
{
	FMedalType medalType;
	_medalsToAnnounce.Dequeue(medalType);

	BpOnAnnounceMedal(medalType);

	if (!_medalsToAnnounce.IsEmpty())
	{
		CreateMedalAnnouncementTask(_consequentAnnouncementDelay);
	}
}
