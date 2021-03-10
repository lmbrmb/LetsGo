#include "AnnouncementManagerComponent.h"
#include "Logs/DevLogger.h"
#include "GameFramework/GameModeBase.h"

void UAnnouncementManagerComponent::BeginPlay()
{
}

UAnnouncementManagerComponent::UAnnouncementManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAnnouncementManagerComponent::SetPlayerId(int32 playerId)
{
	_playerId = playerId;
}

void UAnnouncementManagerComponent::OnMatchHighlight(const int32 playerId, const FMatchHighlight matchHighlight)
{
	if(playerId != _playerId)
	{
		return;
	}
	
	auto const queueWasEmpty = _matchHighlights.IsEmpty();
	_matchHighlights.Enqueue(matchHighlight);
	
	if (!queueWasEmpty)
	{
		return;
	}

	CreateAnnouncementTask(_announcementDelay);
}

void UAnnouncementManagerComponent::AnnounceOnTimer()
{
	FMatchHighlight matchHighlight;
	_matchHighlights.Dequeue(matchHighlight);
	
	if(matchHighlight == FMatchHighlight::None)
	{
		DevLogger::GetLoggingChannel()->Log("Match highlight is None", LogSeverity::Warning);
		return;
	}
	
	BpAnnounce(matchHighlight);

	if(!_matchHighlights.IsEmpty())
	{
		CreateAnnouncementTask(_consequentAnnouncementDelay);
	}
}

void UAnnouncementManagerComponent::CreateAnnouncementTask(const float delay)
{
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	FTimerHandle announceTimerHandle;
	authGameMode->GetWorldTimerManager().SetTimer(
		announceTimerHandle, 
		this, 
		&UAnnouncementManagerComponent::AnnounceOnTimer, 
		delay, 
		false
	);
}
