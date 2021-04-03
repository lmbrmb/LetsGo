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

	_announcementProcessors.Add([this](auto announcement) { return this->TryProcessMedalAnnouncement(announcement); });
	_announcementProcessors.Add([this](auto announcement) { return this->TryProcessFragAnnouncement(announcement); });
}

void UAnnouncementManagerComponent::SetPlayerId(const PlayerId& playerId)
{
	_playerId = playerId;
}

void UAnnouncementManagerComponent::OnMatchWarmUp()
{
	MatchWarmUpAnnouncementRequest.Broadcast();
	CreateAllMatchAnnouncementsDoneTask(_matchWarmUpAnnouncementDuration);
}

void UAnnouncementManagerComponent::OnMatchStart()
{
	MatchStartAnnouncementRequest.Broadcast();
	CreateAllMatchAnnouncementsDoneTask(_matchStartAnnouncementDuration);
}

void UAnnouncementManagerComponent::OnMatchEnd()
{
	MatchEndAnnouncementRequest.Broadcast();
	CreateAllMatchAnnouncementsDoneTask(_matchEndAnnouncementDuration);
}

void UAnnouncementManagerComponent::OnMedalAchieved(const Medal& medal)
{
	if(medal.GetPlayerId() != _playerId)
	{
		return;
	}
	
	auto const medalType = medal.GetMedalType();
	auto const medalAnnouncement = new MedalAnnouncement(medalType);
	AddPlayerAnnouncement(medalAnnouncement);
}

void UAnnouncementManagerComponent::OnPlayerFragged(
	const PlayerId& instigatorPlayerId,
	const PlayerId& fraggedPlayerId,
	const FName& instigatorPlayerNickname,
	const FName& fraggedPlayerNickname
)
{
	auto const isLocalPlayerInstigator = _playerId == instigatorPlayerId;
	auto const isLocalPlayerFragged = _playerId == fraggedPlayerId;
	auto const isRelevantToLocalPlayer = isLocalPlayerInstigator || isLocalPlayerFragged;

	if(isRelevantToLocalPlayer)
	{
		auto const fragAnnouncement = new FragAnnouncement(instigatorPlayerNickname, fraggedPlayerNickname, isLocalPlayerInstigator, isLocalPlayerFragged);
		AddPlayerAnnouncement(fragAnnouncement);
	}
}

void UAnnouncementManagerComponent::AddPlayerAnnouncement(Announcement* announcement)
{
	_playerAnnouncements.Enqueue(announcement);

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
	
	CreatePlayerAnnouncementTask(delay);

	if (_announcementDoneTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_announcementDoneTimerHandle);
		AssertIsFalse(_announcementDoneTimerHandle.IsValid());
	}

	GetWorld()->GetTimerManager().SetTimer(
		_announcementDoneTimerHandle,
		this,
		&UAnnouncementManagerComponent::AllPlayerAnnouncementsDoneOnTimer,
		delay + _playerAnnouncementDuration,
		false
	);
}

void UAnnouncementManagerComponent::CreatePlayerAnnouncementTask(const float delay)
{
	FTimerHandle playerAnnounceTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		playerAnnounceTimerHandle,
		this, 
		&UAnnouncementManagerComponent::PlayerAnnouncementOnTimer, 
		delay, 
		false
	);
}

void UAnnouncementManagerComponent::CreateAllMatchAnnouncementsDoneTask(const float delay)
{
	FTimerHandle allMatchAnnouncementsDoneTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		allMatchAnnouncementsDoneTimerHandle,
		this,
		&UAnnouncementManagerComponent::AllMatchAnnouncementsDoneOnTimer,
		delay,
		false
	);
}

void UAnnouncementManagerComponent::PlayerAnnouncementOnTimer()
{
	Announcement* announcement;
	_playerAnnouncements.Dequeue(announcement);

	AssertIsNotNull(announcement);
	
	ProcessAnnouncement(announcement);
	delete announcement;
}

void UAnnouncementManagerComponent::AllPlayerAnnouncementsDoneOnTimer() const
{
	AllPlayerAnnouncementsDone.Broadcast();
}

void UAnnouncementManagerComponent::AllMatchAnnouncementsDoneOnTimer() const
{
	AllMatchAnnouncementsDone.Broadcast();
}

void UAnnouncementManagerComponent::ProcessAnnouncement(Announcement* announcement)
{
	AssertIsNotNull(announcement);
	for (auto announcementProcessor : _announcementProcessors)
	{
		auto const isProcessed(announcementProcessor(announcement));
		if (isProcessed)
		{
			return;
		}
	}

	DevLogger::GetLoggingChannel()->Log("Announcement is not processed", LogSeverity::Error);
}

bool UAnnouncementManagerComponent::TryProcessMedalAnnouncement(Announcement* announcement) const
{
	auto const medalAnnouncement = dynamic_cast<MedalAnnouncement*>(announcement);

	if (!medalAnnouncement)
	{
		return false;
	}

	MedalAnnouncementRequest.Broadcast(medalAnnouncement);

	return true;
}

bool UAnnouncementManagerComponent::TryProcessFragAnnouncement(Announcement* announcement) const
{
	auto const fragAnnouncement = dynamic_cast<FragAnnouncement*>(announcement);
	if (!fragAnnouncement)
	{
		return false;
	}
	
	FragAnnouncementRequest.Broadcast(fragAnnouncement);

	return true;
}
