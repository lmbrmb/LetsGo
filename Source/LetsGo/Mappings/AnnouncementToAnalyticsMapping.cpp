#include "AnnouncementToAnalyticsMapping.h"

#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementToAnalyticsMapping::Map()
{
	auto const owner = GetOwner();
	
	auto const announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(announcementManagerComponent);

	auto const world = GetWorld();
	auto const authGameMode = world->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(matchGameMode);

	matchGameMode->PlayerFragged.AddUObject(announcementManagerComponent, &UAnnouncementManagerComponent::OnPlayerFragged);

	auto const matchAnalytics = matchGameMode->GetMatchAnalytics();
	AssertIsNotNull(matchAnalytics);

	matchAnalytics->MedalAchieved.AddUObject(announcementManagerComponent, &UAnnouncementManagerComponent::OnMedalAchieved);
	StartDestroyTask();
}
