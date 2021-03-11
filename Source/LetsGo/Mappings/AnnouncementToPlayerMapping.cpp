#include "AnnouncementToPlayerMapping.h"

#include "GameFramework/PlayerState.h"
#include "LetsGo/AnnouncementManagerComponent.h"
#include "LetsGo/PlayerControllers/MatchPlayerController.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementToPlayerMapping::Map()
{
	auto const owner = GetOwner();
	
	auto const matchPlayerController = Cast<AMatchPlayerController>(owner);
	AssertIsNotNull(matchPlayerController);

	auto const announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(announcementManagerComponent);

	auto const playerState = matchPlayerController->GetPlayerState<APlayerState>();
	AssertIsNotNull(playerState);
	
	auto const playerId =playerState->GetPlayerId();
	announcementManagerComponent->SetPlayerId(playerId);
	DestroyComponent();
}
