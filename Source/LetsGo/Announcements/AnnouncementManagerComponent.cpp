#include "AnnouncementManagerComponent.h"

#include "MedalAnnouncement.h"
#include "MessageAnnouncement.h"
#include "GameFramework/GameModeBase.h"
#include "LetsGo/Logs/DevLogger.h"

UAnnouncementManagerComponent::UAnnouncementManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAnnouncementManagerComponent::BeginPlay()
{
	_announcementProcessors.Add([this](auto announcement) { return this->TryProcessMedalAnnouncement(announcement); });
	_announcementProcessors.Add([this](auto announcement) { return this->TryProcessMessageAnnouncement(announcement); });
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
	
	auto const queueWasEmpty = _announcements.IsEmpty();
	auto const medalType = medal.GetMedalType();
	auto const medalAnnouncement = new MedalAnnouncement(medalType);
	_announcements.Enqueue(medalAnnouncement);
	
	if (!queueWasEmpty)
	{
		return;
	}
	
	CreateAnnouncementTask(_announcementDelay);
}

// ReSharper disable once CppMemberFunctionMayBeConst
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
	auto const isSuicide = instigatorPlayerId == fraggedPlayerId;

	FString message;

	if (isRelevantToLocalPlayer)
	{
		if (isSuicide)
		{
			message = "Suicide";
		}
		else if (isLocalPlayerInstigator)
		{
			message = "You fragged " + fraggedPlayerNickname.ToString();
		}
		else
		{
			message = "Fragged by " + instigatorPlayerNickname.ToString();
		}
	}
	else
	{
		if (isSuicide)
		{
			message = instigatorPlayerNickname.ToString() + " killed himself";
		}
		else
		{
			message = instigatorPlayerNickname.ToString() + " fragged " + fraggedPlayerNickname.ToString();
		}
	}

	auto const messageAnnouncement = new MessageAnnouncement(message, isRelevantToLocalPlayer);
	auto const queueWasEmpty = _announcements.IsEmpty();
	_announcements.Enqueue(messageAnnouncement);

	if(!queueWasEmpty)
	{
		return;
	}
	
	CreateAnnouncementTask(_announcementDelay);
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

void UAnnouncementManagerComponent::AnnounceOnTimer()
{
	Announcement* announcement;
	_announcements.Dequeue(announcement);

	ProcessAnnouncement(announcement);

	delete announcement;

	if (_announcements.IsEmpty())
	{
		return;
	}

	CreateAnnouncementTask(_consequentAnnouncementDelay);
}

void UAnnouncementManagerComponent::ProcessAnnouncement(Announcement* announcement) const
{
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

	auto const medalType = medalAnnouncement->GetMedalType();
	MedalAnnouncementRequest.Broadcast(medalType);
	
	FString medalMessage;
	switch (medalType)
	{
		case FMedalType::FirstBlood:
			medalMessage = "FIRST BLOOD";
			break;
		case FMedalType::Excellent:
			medalMessage = "EXCELLENT";
			break;
		case FMedalType::Impressive:
			medalMessage = "IMPRESSIVE";
			break;
		default:
			DevLogger::GetLoggingChannel()->LogValue("Unknown FMedalType", (int)medalType, LogSeverity::Warning);
			break;
	}

	if (!medalMessage.IsEmpty())
	{
		MessageAnnouncementRequest.Broadcast(medalMessage);
	}

	return true;
}

bool UAnnouncementManagerComponent::TryProcessMessageAnnouncement(Announcement* announcement) const
{
	auto const messageAnnouncement = dynamic_cast<MessageAnnouncement*>(announcement);
	if (!messageAnnouncement)
	{
		return false;
	}

	auto const message = messageAnnouncement->GetMessage();
	MessageAnnouncementRequest.Broadcast(message);

	return true;
}
