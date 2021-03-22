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

void UAnnouncementManagerComponent::OnMedalAchieved(const Medal& medal)
{
	if(medal.GetPlayerId() != _playerId)
	{
		return;
	}
	
	auto const medalType = medal.GetMedalType();
	auto const medalAnnouncement = new MedalAnnouncement(medalType);
	AddAnnouncement(medalAnnouncement);
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
		AddAnnouncement(fragAnnouncement);
	}
}

void UAnnouncementManagerComponent::AddAnnouncement(Announcement* announcement)
{
	_announcements.Enqueue(announcement);

	float delay;
	auto const now = GetWorld()->TimeSeconds;
	
	if (now > _nextAnnouncementTime)
	{
		delay = _firstAnnouncementDelay;
	}
	else
	{
		delay = _nextAnnouncementTime - now + _announcementDuration;
	}

	_nextAnnouncementTime = now + delay;
	
	CreateAnnouncementTask(delay);

	if (_announcementDoneTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_announcementDoneTimerHandle);
		AssertIsFalse(_announcementDoneTimerHandle.IsValid())
	}

	GetWorld()->GetTimerManager().SetTimer(
		_announcementDoneTimerHandle,
		this,
		&UAnnouncementManagerComponent::AllAnnouncementsDoneOnTimer,
		delay + _announcementDuration,
		false
	);
}

void UAnnouncementManagerComponent::CreateAnnouncementTask(const float delay)
{
	FTimerHandle announceTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		announceTimerHandle,
		this, 
		&UAnnouncementManagerComponent::AnnounceOnTimer, 
		delay, 
		false
	);
}

void UAnnouncementManagerComponent::AnnounceOnTimer()
{
	Announcement* announcement;
	_announcements.Dequeue(announcement);

	AssertIsNotNull(announcement);
	
	ProcessAnnouncement(announcement);
	delete announcement;
}

void UAnnouncementManagerComponent::AllAnnouncementsDoneOnTimer() const
{
	AllAnnouncementsDone.Broadcast();
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
