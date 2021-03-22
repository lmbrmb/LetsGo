#include "AnnouncementToGameModeMapping.h"

#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementToGameModeMapping::Map()
{
	auto const owner = GetOwner();

	auto const announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(announcementManagerComponent);

	auto const world = GetWorld();
	auto const authGameMode = world->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(matchGameMode);

	matchGameMode->PlayerFragged.AddUObject(announcementManagerComponent, &UAnnouncementManagerComponent::OnPlayerFragged);
	
	SubscribeMatchStateEvents(matchGameMode, announcementManagerComponent);

	StartDestroyTask();
}

void UAnnouncementToGameModeMapping::SubscribeMatchStateEvents(
	AMatchGameMode* matchGameMode,
	UAnnouncementManagerComponent* announcementManagerComponent
)
{
	if (matchGameMode->IsMatchEnded())
	{
		announcementManagerComponent->OnMatchEnd();
		return;
	}

	matchGameMode->MatchEnd.AddUObject(announcementManagerComponent, &UAnnouncementManagerComponent::OnMatchEnd);

	if (matchGameMode->IsMatchStarted())
	{
		announcementManagerComponent->OnMatchStart();
		return;
	}

	matchGameMode->MatchStart.AddUObject(announcementManagerComponent, &UAnnouncementManagerComponent::OnMatchStart);

	if (matchGameMode->IsMatchWarmUp())
	{
		announcementManagerComponent->OnMatchWarmUp();
		return;
	}

	matchGameMode->MatchWarmUp.AddUObject(announcementManagerComponent, &UAnnouncementManagerComponent::OnMatchWarmUp);
}
