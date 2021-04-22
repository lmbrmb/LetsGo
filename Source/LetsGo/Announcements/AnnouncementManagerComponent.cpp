#include "AnnouncementManagerComponent.h"

#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

UAnnouncementManagerComponent::UAnnouncementManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAnnouncementManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	_matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(_matchGameMode);

	_matchWarmUpAnnouncementFactory = new MatchWarmUpAnnouncementFactory();
	_matchStartAnnouncementFactory = new MatchStartAnnouncementFactory();
	_matchEndAnnouncementFactory = new MatchEndAnnouncementFactory();
}

void UAnnouncementManagerComponent::SetPlayerId(const PlayerId& playerId)
{
	_playerId = playerId;
}

void UAnnouncementManagerComponent::OnMatchWarmUp()
{
	auto const matchWarmUpAnnouncement = _matchWarmUpAnnouncementFactory->Create();
	AssertIsNotNull(matchWarmUpAnnouncement);
	AddAnnouncement(matchWarmUpAnnouncement);
}

void UAnnouncementManagerComponent::OnMatchStart()
{
	auto const matchStartAnnouncement = _matchStartAnnouncementFactory->Create();
	AssertIsNotNull(matchStartAnnouncement);
	AddAnnouncement(matchStartAnnouncement);
}

void UAnnouncementManagerComponent::OnMatchEnd()
{
	auto const matchEndAnnouncement = _matchEndAnnouncementFactory->Create(_playerId, _matchGameMode);
	AssertIsNotNull(matchEndAnnouncement);
	AddAnnouncement(matchEndAnnouncement);
}

void UAnnouncementManagerComponent::OnMedalAchieved(const Medal& medal)
{
	auto const medalAnnouncement = _medalAnnouncementFactory->Create(medal, _playerId);

	if(!medalAnnouncement)
	{
		return;
	}
	
	AddAnnouncement(medalAnnouncement);
}

void UAnnouncementManagerComponent::OnPlayerFragged(
	const PlayerId& instigatorPlayerId,
	const PlayerId& fraggedPlayerId
)
{
	auto const fragAnnouncement = _fragAnnouncementFactory->Create(_playerId, instigatorPlayerId, fraggedPlayerId, _matchGameMode);

	if(!fragAnnouncement)
	{
		return;
	}

	AddAnnouncement(fragAnnouncement);
}

void UAnnouncementManagerComponent::AddAnnouncement(IAnnouncement* announcement)
{
	_announcements.Enqueue(announcement);

	float delay;
	auto const now = GetWorld()->TimeSeconds;
	
	if (now > _nextAnnouncementTime)
	{
		delay = _firstPlayerAnnouncementDelay;
	}
	else
	{
		delay = _nextAnnouncementTime - now + _playerAnnouncementDuration;
	}

	_nextAnnouncementTime = now + delay;
	
	CreateAnnouncementTask(delay);

	if (_announcementDoneTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_announcementDoneTimerHandle);
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		_announcementDoneTimerHandle,
		this,
		&UAnnouncementManagerComponent::AllAnnouncementsDoneOnTimer,
		delay + _playerAnnouncementDuration,
		false
	);
}

void UAnnouncementManagerComponent::CreateAnnouncementTask(const float delay)
{
	FTimerHandle playerAnnounceTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		playerAnnounceTimerHandle,
		this, 
		&UAnnouncementManagerComponent::AnnounceOnTimer, 
		delay, 
		false
	);
}

void UAnnouncementManagerComponent::AnnounceOnTimer()
{
	IAnnouncement* announcement;
	_announcements.Dequeue(announcement);

	AssertIsNotNull(announcement);

	AnnouncementRequest.Broadcast(announcement);
	
	delete announcement;
}

void UAnnouncementManagerComponent::AllAnnouncementsDoneOnTimer() const
{
	AllAnnouncementsDone.Broadcast();
}
