#include "AnnouncementManagerToMatchAnalyticsMapping.h"

#include "LetsGo/AnnouncementManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementManagerToMatchAnalyticsMapping::Map()
{
	auto const owner = GetOwner();
	auto const announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();

	AssertIsNotNull(announcementManagerComponent);
	
	auto const world = GetWorld();
	auto const authGameMode = world->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);

	AssertIsNotNull(matchGameMode);

	auto const matchAnalytics = matchGameMode->GetMatchAnalytics();

	AssertIsNotNull(matchAnalytics);

	matchAnalytics->MatchHighlight.AddUObject(announcementManagerComponent, &UAnnouncementManagerComponent::OnMatchHighlight);
}
