#include "AnnouncementToPlayerMapping.h"

#include "GameFramework/PlayerState.h"
#include "LetsGo/PlayerControllers/MatchPlayerController.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementToPlayerMapping::Map()
{
	auto const owner = GetOwner();

	_announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(_announcementManagerComponent);

	if (_announcementManagerComponent->IsInitialized())
	{
		OnAnnouncementManagerComponentInitialized();
		return;
	}

	_announcementManagerComponent->Initialized.AddUObject(this, &UAnnouncementToPlayerMapping::OnAnnouncementManagerComponentInitialized);
}

void UAnnouncementToPlayerMapping::OnAnnouncementManagerComponentInitialized()
{
	_announcementManagerComponent->Initialized.RemoveAll(this);

	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	AssertIsNotNull(announcementManager);

	auto const owner = GetOwner();
	auto const matchPlayerController = Cast<AMatchPlayerController>(owner);
	AssertIsNotNull(matchPlayerController);

	auto const playerState = matchPlayerController->GetPlayerState<APlayerState>();
	AssertIsNotNull(playerState);

	auto const playerIdValue = playerState->GetPlayerId();
	auto const playerId = PlayerId(playerIdValue);

	announcementManager->SetPlayerId(playerId);

	StartDestroyTask();
}
