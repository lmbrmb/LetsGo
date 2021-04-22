#include "AnnouncementManager.h"

#include "LetsGo/Utils/AssertUtils.h"

const float AnnouncementManager::UNDEFINED_TIME = -1;

AnnouncementManager::AnnouncementManager(
	AMatchGameMode* matchGameMode
) : _matchGameMode(matchGameMode)
{
}

void AnnouncementManager::SetPlayerId(const PlayerId& playerId)
{
	_playerId = playerId;
}

void AnnouncementManager::SetFactories(
	FragAnnouncementFactory* fragAnnouncementFactory,
	MedalAnnouncementFactory* medalAnnouncementFactory,
	MatchWarmUpAnnouncementFactory* matchWarmUpAnnouncementFactory,
	MatchStartAnnouncementFactory* matchStartAnnouncementFactory,
	MatchEndAnnouncementFactory* matchEndAnnouncementFactory)
{
	_fragAnnouncementFactory = fragAnnouncementFactory;
	_medalAnnouncementFactory = medalAnnouncementFactory;
	_matchWarmUpAnnouncementFactory = matchWarmUpAnnouncementFactory;
	_matchStartAnnouncementFactory = matchStartAnnouncementFactory;
	_matchEndAnnouncementFactory = matchEndAnnouncementFactory;
}

void AnnouncementManager::SetTimings(
	const float matchWarmUpAnnouncementDuration,
	const float matchStartAnnouncementDuration,
	const float matchEndAnnouncementDuration,
	const float firstPlayerAnnouncementDelay,
	const float playerAnnouncementDuration
)
{
	_matchWarmUpAnnouncementDuration = matchWarmUpAnnouncementDuration;
	_matchStartAnnouncementDuration = matchStartAnnouncementDuration;
	_matchEndAnnouncementDuration = matchEndAnnouncementDuration;
	_firstPlayerAnnouncementDelay = firstPlayerAnnouncementDelay;
	_playerAnnouncementDuration = playerAnnouncementDuration;
}

void AnnouncementManager::OnMatchWarmUp()
{
	auto const matchWarmUpAnnouncement = _matchWarmUpAnnouncementFactory->Create();
	AssertIsNotNull(matchWarmUpAnnouncement);
	AddAnnouncement(matchWarmUpAnnouncement);
}

void AnnouncementManager::OnMatchStart()
{
	auto const matchStartAnnouncement = _matchStartAnnouncementFactory->Create();
	AssertIsNotNull(matchStartAnnouncement);
	AddAnnouncement(matchStartAnnouncement);
}

void AnnouncementManager::OnMatchEnd()
{
	auto const matchEndAnnouncement = _matchEndAnnouncementFactory->Create(_playerId, _matchGameMode);
	AssertIsNotNull(matchEndAnnouncement);
	AddAnnouncement(matchEndAnnouncement);
}

void AnnouncementManager::OnMedalAchieved(const Medal& medal)
{
	auto const medalAnnouncement = _medalAnnouncementFactory->Create(medal, _playerId);

	if (!medalAnnouncement)
	{
		return;
	}

	AddAnnouncement(medalAnnouncement);
}

void AnnouncementManager::OnPlayerFragged(
	const PlayerId& instigatorPlayerId,
	const PlayerId& fraggedPlayerId
)
{
	auto const fragAnnouncement = _fragAnnouncementFactory->Create(_playerId, instigatorPlayerId, fraggedPlayerId, _matchGameMode);

	if (!fragAnnouncement)
	{
		return;
	}

	AddAnnouncement(fragAnnouncement);
}

void AnnouncementManager::AddAnnouncement(IAnnouncement* announcement)
{
	_announcements.Enqueue(announcement);

	float delay;
	auto const now = _matchGameMode->GetWorld()->TimeSeconds;

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
	CreateAllAnnouncementsDoneTask(delay);
}

void AnnouncementManager::CreateAnnouncementTask(const float delay)
{
	AssertIsGreaterOrEqual(delay, 0.0f);

	if(delay > 0)
	{
		FTimerHandle announceTimerHandle;
		_matchGameMode->GetWorld()->GetTimerManager().SetTimer(
			announceTimerHandle,
			[this]() { AnnounceOnTimer(); },
			delay,
			false
		);
		return;
	}
	
	AnnounceOnTimer();
}

void AnnouncementManager::CreateAllAnnouncementsDoneTask(const float delay)
{
	if (_announcementDoneTimerHandle.IsValid())
	{
		_matchGameMode->GetWorld()->GetTimerManager().ClearTimer(_announcementDoneTimerHandle);
	}

	auto const clearAnnouncementsDelay = delay + _playerAnnouncementDuration;
	_matchGameMode->GetWorld()->GetTimerManager().SetTimer(
		_announcementDoneTimerHandle,
		[this]() { AllAnnouncementsDoneOnTimer(); },
		clearAnnouncementsDelay,
		false
	);
}

void AnnouncementManager::AnnounceOnTimer()
{
	IAnnouncement* announcement;
	_announcements.Dequeue(announcement);

	AssertIsNotNull(announcement);

	AnnouncementRequest.Broadcast(announcement);

	delete announcement;
}

void AnnouncementManager::AllAnnouncementsDoneOnTimer() const
{
	AllAnnouncementsDone.Broadcast();
}
