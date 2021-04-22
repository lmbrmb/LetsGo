#include "AnnouncementToGameModeMapping.h"

#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementToGameModeMapping::Map()
{
	auto const owner = GetOwner();

	_announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(_announcementManagerComponent);

	if(_announcementManagerComponent->IsInitialized())
	{
		OnAnnouncementManagerComponentInitialized();
		return;
	}

	_announcementManagerComponent->Initialized.AddUObject(this, &UAnnouncementToGameModeMapping::OnAnnouncementManagerComponentInitialized);
}

void UAnnouncementToGameModeMapping::OnAnnouncementManagerComponentInitialized()
{
	_announcementManagerComponent->Initialized.RemoveAll(this);

	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	AssertIsNotNull(announcementManager);
	
	auto const world = GetWorld();
	auto const authGameMode = world->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(matchGameMode);

	matchGameMode->PlayerFragged.AddRaw(announcementManager, &IAnnouncementManager::OnPlayerFragged);

	SubscribeMatchStateEvents(matchGameMode, announcementManager);

	StartDestroyTask();
}

void UAnnouncementToGameModeMapping::SubscribeMatchStateEvents(
	AMatchGameMode* matchGameMode,
	IAnnouncementManager* announcementManager
)
{
	if (matchGameMode->IsMatchEnded())
	{
		announcementManager->OnMatchEnd();
		return;
	}

	matchGameMode->MatchEnd.AddRaw(announcementManager, &IAnnouncementManager::OnMatchEnd);

	if (matchGameMode->IsMatchStarted())
	{
		announcementManager->OnMatchStart();
		return;
	}

	matchGameMode->MatchStart.AddRaw(announcementManager, &IAnnouncementManager::OnMatchStart);

	if (matchGameMode->IsMatchWarmUp())
	{
		announcementManager->OnMatchWarmUp();
		return;
	}

	matchGameMode->MatchWarmUp.AddRaw(announcementManager, &IAnnouncementManager::OnMatchWarmUp);
}
