#include "AnnouncementToAnalyticsMapping.h"

#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Analytics/MatchAnalytics.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementToAnalyticsMapping::Map()
{
	auto const owner = GetOwner();
	
	_announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(_announcementManagerComponent);

	if(_announcementManagerComponent->IsInitialized())
	{
		OnAnnouncementManagerComponentInitialized();
		return;
	}

	_announcementManagerComponent->Initialized.AddUObject(this, &UAnnouncementToAnalyticsMapping::OnAnnouncementManagerComponentInitialized);
}

void UAnnouncementToAnalyticsMapping::OnAnnouncementManagerComponentInitialized()
{
	_announcementManagerComponent->Initialized.RemoveAll(this);

	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	AssertIsNotNull(announcementManager);

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(matchGameMode);

	auto const matchAnalytics = matchGameMode->GetMatchAnalytics();
	AssertIsNotNull(matchAnalytics);

	matchAnalytics->MedalAchieved.AddRaw(announcementManager, &IAnnouncementManager::OnMedalAchieved);

	StartDestroyTask();
}
