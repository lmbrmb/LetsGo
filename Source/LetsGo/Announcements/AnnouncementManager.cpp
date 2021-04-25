#include "AnnouncementManager.h"

#include "LetsGo/Utils/AssertUtils.h"

const float AnnouncementManager::UNDEFINED_TIME = -1;

AnnouncementManager::AnnouncementManager(
	AMatchGameMode* matchGameMode,
	const PlayerId& playerId
) :
	_matchGameMode(matchGameMode),
	_playerId(playerId)
{
}

void AnnouncementManager::SetFactories(
	FragAnnouncementFactory* fragAnnouncementFactory,
	MedalAnnouncementFactory* medalAnnouncementFactory,
	MatchWarmUpAnnouncementFactory* matchWarmUpAnnouncementFactory,
	MatchStartAnnouncementFactory* matchStartAnnouncementFactory,
	MatchEndAnnouncementFactory* matchEndAnnouncementFactory,
	LeadAnnouncementFactory* leadAnnouncementFactory
)
{
	_fragAnnouncementFactory = fragAnnouncementFactory;
	_medalAnnouncementFactory = medalAnnouncementFactory;
	_matchWarmUpAnnouncementFactory = matchWarmUpAnnouncementFactory;
	_matchStartAnnouncementFactory = matchStartAnnouncementFactory;
	_matchEndAnnouncementFactory = matchEndAnnouncementFactory;
	_leadAnnouncementFactory = leadAnnouncementFactory;
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
	AssertIsNotNull(_matchWarmUpAnnouncementFactory);
	auto const matchWarmUpAnnouncement = _matchWarmUpAnnouncementFactory->Create();
	AssertIsNotNull(matchWarmUpAnnouncement);
	AddAnnouncement(matchWarmUpAnnouncement);
}

void AnnouncementManager::OnMatchStart()
{
	AssertIsNotNull(_matchStartAnnouncementFactory);
	auto const matchStartAnnouncement = _matchStartAnnouncementFactory->Create();
	AssertIsNotNull(matchStartAnnouncement);
	AddAnnouncement(matchStartAnnouncement);
}

void AnnouncementManager::OnMatchEnd()
{
	AssertIsNotNull(_matchEndAnnouncementFactory);
	auto const matchEndAnnouncement = _matchEndAnnouncementFactory->Create(_playerId, _matchGameMode);
	AssertIsNotNull(matchEndAnnouncement);
	AddAnnouncement(matchEndAnnouncement);
}

void AnnouncementManager::OnMedalAchieved(const Medal& medal)
{
	AssertIsNotNull(_medalAnnouncementFactory);
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
	CheckLead();
	CreateFragAnnouncement(instigatorPlayerId, fraggedPlayerId);
}

void AnnouncementManager::CheckLead()
{
	auto maxFrags = 0;
	auto thisPlayerFrags = 0;
	auto const thisPlayerIdValue = _playerId.GetId();
	
	for (auto const playerFragRecord : _matchGameMode->GetPlayerFrags())
	{
		auto const playerIdValue = playerFragRecord.Key;
		auto const frags = playerFragRecord.Value;

		if (maxFrags < frags)
		{
			maxFrags = frags;
		}

		if(thisPlayerIdValue == playerIdValue)
		{
			thisPlayerFrags = frags;
		}
	}
	
	auto maxFragsCount = 0;

	for (auto const playerFragRecord : _matchGameMode->GetPlayerFrags())
	{
		auto const fragCount = playerFragRecord.Value;
		if (fragCount == maxFrags)
		{
			maxFragsCount++;
		}
	}
	
	auto leadState = FLeadState::None;

	if(thisPlayerFrags == maxFrags)
	{
		if(maxFragsCount == 1)
		{
			leadState = FLeadState::Taken;
		}
		else
		{
			leadState = FLeadState::Tied;
		}
	}
	else
	{
		leadState = FLeadState::Lost;
	}

	if(_leadState != leadState)
	{
		_leadState = leadState;
		AssertIsNotNull(_leadAnnouncementFactory);
		auto const leadAnnouncement = _leadAnnouncementFactory->Create(_leadState);
		AssertIsNotNull(leadAnnouncement);
		AddAnnouncement(leadAnnouncement);
	}
}

void AnnouncementManager::CreateFragAnnouncement(
	const PlayerId& instigatorPlayerId,
	const PlayerId& fraggedPlayerId
)
{
	AssertIsNotNull(_fragAnnouncementFactory);
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
	_matchGameMode->GetWorld()->GetTimerManager().ClearTimer(_announcementDoneTimerHandle);

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
	if(!IsContextValid())
	{
		return;
	}

	IAnnouncement* announcement;
	_announcements.Dequeue(announcement);

	AssertIsNotNull(announcement);

	AnnouncementRequest.Broadcast(announcement);

	delete announcement;
}

void AnnouncementManager::AllAnnouncementsDoneOnTimer() const
{
	if(!IsContextValid())
	{
		return;
	}

	AllAnnouncementsDone.Broadcast();
}

bool AnnouncementManager::IsContextValid() const
{
	return _matchGameMode && _matchGameMode->IsValidLowLevel();
}
