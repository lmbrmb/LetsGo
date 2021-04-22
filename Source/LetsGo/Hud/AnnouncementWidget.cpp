#include "AnnouncementWidget.h"

#include "AnnouncementProcessors/FragAnnouncementProcessor.h"
#include "AnnouncementProcessors/MatchEndAnnouncementProcessor.h"
#include "AnnouncementProcessors/MatchStartAnnouncementProcessor.h"
#include "AnnouncementProcessors/MatchWarmUpAnnouncementProcessor.h"
#include "AnnouncementProcessors/MedalAnnouncementProcessor.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	_announcementProcessors.Add(new FragAnnouncementProcessor());
	_announcementProcessors.Add(new MedalAnnouncementProcessor());
	_announcementProcessors.Add(new MatchWarmUpAnnouncementProcessor());
	_announcementProcessors.Add(new MatchStartAnnouncementProcessor());
	_announcementProcessors.Add(new MatchEndAnnouncementProcessor());
}

void UAnnouncementWidget::OnAnnouncementRequest(const IAnnouncement* announcement)
{
	AssertIsNotNull(announcement);

	AssertIsNotNull(announcement);
	for (auto announcementProcessor : _announcementProcessors)
	{
		auto const isProcessed = announcementProcessor->TryProcessAnnouncement(announcement, this);
		if (isProcessed)
		{
			return;
		}
	}

	DevLogger::GetLoggingChannel()->Log("Announcement is not processed", LogSeverity::Error);
}

void UAnnouncementWidget::OnAllAnnouncementsDone()
{
	BpAllAnnouncementsDone();
}

void UAnnouncementWidget::AnnounceMatchWarmUp()
{
	BpAnnounceMatchWarmUp();
}

void UAnnouncementWidget::AnnounceMatchStart()
{
	BpAnnounceMatchStart();
}

void UAnnouncementWidget::AnnounceMatchEnd(const int localPlayerPlace)
{
	BpAnnounceMatchEnd(localPlayerPlace);
}

void UAnnouncementWidget::AnnounceFrag(
	const FName& instigatorPlayerNickname,
	const FName& fraggedPlayerNickname,
	const bool isLocalPlayerInstigator,
	const bool isLocalPlayerFragged,
	const int instigatorPlayerPlace,
	const int fraggedPlayerPlace
)
{
	BpAnnounceFrag(
		instigatorPlayerNickname,
		fraggedPlayerNickname,
		isLocalPlayerInstigator,
		isLocalPlayerFragged,
		instigatorPlayerPlace,
		fraggedPlayerPlace
	);
}

void UAnnouncementWidget::AnnounceMedal(const FMedalType medalType)
{
	BpAnnounceMedal(medalType);
}

void UAnnouncementWidget::ProcessAnnouncement(IAnnouncement* announcement)
{
	AssertIsNotNull(announcement);
	for (auto announcementProcessor : _announcementProcessors)
	{
		auto const isProcessed = announcementProcessor->TryProcessAnnouncement(announcement, this);
		if (isProcessed)
		{
			return;
		}
	}

	DevLogger::GetLoggingChannel()->Log("Announcement is not processed", LogSeverity::Error);
}
