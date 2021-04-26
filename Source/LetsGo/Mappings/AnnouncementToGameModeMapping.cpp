#include "AnnouncementToGameModeMapping.h"

#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementToGameModeMapping::Map()
{
	auto const world = GetWorld();
	auto const authGameMode = world->GetAuthGameMode();
	_matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(_matchGameMode);

	_matchGameMode->Exit.AddUObject(this, &UAnnouncementToGameModeMapping::OnExit);

	if (_matchGameMode->IsMatchEnded())
	{
		OnMatchEnd();
		return;
	}

	_matchGameMode->MatchEnd.AddUObject(this, &UAnnouncementToGameModeMapping::OnMatchEnd);

	auto const owner = GetOwner();
	_announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(_announcementManagerComponent);

	if(_announcementManagerComponent->IsInitialized())
	{
		OnAnnouncementManagerComponentInitialized();
	}
	else
	{
		_announcementManagerComponent->Initialized.AddUObject(this, &UAnnouncementToGameModeMapping::OnAnnouncementManagerComponentInitialized);
	}
}

void UAnnouncementToGameModeMapping::OnAnnouncementManagerComponentInitialized() const
{
	_announcementManagerComponent->Initialized.RemoveAll(this);

	BindAnnouncementManager();
}

void UAnnouncementToGameModeMapping::BindAnnouncementManager() const
{
	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	AssertIsNotNull(announcementManager);

	_matchGameMode->PlayerFragged.AddRaw(announcementManager, &IAnnouncementManager::OnPlayerFragged);

	if (_matchGameMode->IsMatchStarted())
	{
		announcementManager->OnMatchStart();
		return;
	}

	_matchGameMode->MatchStart.AddRaw(announcementManager, &IAnnouncementManager::OnMatchStart);

	if (_matchGameMode->IsMatchWarmUp())
	{
		announcementManager->OnMatchWarmUp();
		return;
	}

	_matchGameMode->MatchWarmUp.AddRaw(announcementManager, &IAnnouncementManager::OnMatchWarmUp);
}

void UAnnouncementToGameModeMapping::UnBindAnnouncementManager() const
{
	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	AssertIsNotNull(announcementManager);
	
	_matchGameMode->PlayerFragged.RemoveAll(announcementManager);
	_matchGameMode->MatchStart.RemoveAll(announcementManager);
	_matchGameMode->MatchWarmUp.RemoveAll(announcementManager);
}

void UAnnouncementToGameModeMapping::OnMatchEnd() const
{
	_matchGameMode->MatchEnd.RemoveAll(this);

	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	if(!announcementManager)
	{
		return;
	}

	UnBindAnnouncementManager();
	announcementManager->ClearAllAnnouncements();
	announcementManager->OnMatchEnd();
}

void UAnnouncementToGameModeMapping::OnExit() const
{
	_matchGameMode->Exit.RemoveAll(this);

	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	if(!announcementManager)
	{
		return;
	}

	UnBindAnnouncementManager();
	announcementManager->ClearAllAnnouncements();
}
