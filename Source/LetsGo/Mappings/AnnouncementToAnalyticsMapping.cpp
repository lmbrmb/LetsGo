#include "AnnouncementToAnalyticsMapping.h"

#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/Analytics/MatchAnalytics.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementToAnalyticsMapping::Map()
{
	auto const owner = GetOwner();
	_announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(_announcementManagerComponent);

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	_matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(_matchGameMode);

	if(_matchGameMode->IsMatchEnded())
	{
		return;
	}

	_matchGameMode->MatchEnd.AddUObject(this, &UAnnouncementToAnalyticsMapping::OnMatchEnd);

	if(_announcementManagerComponent->IsInitialized())
	{
		OnAnnouncementManagerComponentInitialized();
		return;
	}

	_announcementManagerComponent->Initialized.AddUObject(this, &UAnnouncementToAnalyticsMapping::OnAnnouncementManagerComponentInitialized);
}

void UAnnouncementToAnalyticsMapping::OnAnnouncementManagerComponentInitialized() const
{
	_announcementManagerComponent->Initialized.RemoveAll(this);

	BindAnnouncementManager();
}

void UAnnouncementToAnalyticsMapping::BindAnnouncementManager() const
{
	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	AssertIsNotNull(announcementManager);

	auto const matchAnalytics = _matchGameMode->GetMatchAnalytics();
	AssertIsNotNull(matchAnalytics);

	matchAnalytics->MedalAchieved.AddRaw(announcementManager, &IAnnouncementManager::OnMedalAchieved);
}

void UAnnouncementToAnalyticsMapping::UnBindAnnouncementManager() const
{
	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	AssertIsNotNull(announcementManager);

	auto const matchAnalytics = _matchGameMode->GetMatchAnalytics();
	AssertIsNotNull(matchAnalytics);

	matchAnalytics->MedalAchieved.RemoveAll(announcementManager);
	announcementManager->ClearAllAnnouncements();
}

void UAnnouncementToAnalyticsMapping::OnMatchEnd() const
{
	_matchGameMode->MatchEnd.RemoveAll(this);

	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	if (!announcementManager)
	{
		return;
	}

	UnBindAnnouncementManager();
}
