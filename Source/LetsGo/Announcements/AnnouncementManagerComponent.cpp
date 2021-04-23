#include "AnnouncementManagerComponent.h"

#include "AnnouncementManagerFactory.h"
#include "GameFramework/PlayerState.h"
#include "LetsGo/PlayerControllers/MatchPlayerController.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

UAnnouncementManagerComponent::UAnnouncementManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UAnnouncementManagerComponent::~UAnnouncementManagerComponent()
{
	if(_announcementManager)
	{
		delete _announcementManager;
	}
}

IAnnouncementManager* UAnnouncementManagerComponent::GetAnnouncementManager() const
{
	return _announcementManager;
}

bool UAnnouncementManagerComponent::IsInitialized() const
{
	return _announcementManager != nullptr;
}

void UAnnouncementManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(matchGameMode);

	AnnouncementManagerFactory announcementManagerFactory;
	announcementManagerFactory.SetTimings(
		_matchWarmUpAnnouncementDuration,
		_matchStartAnnouncementDuration,
		_matchEndAnnouncementDuration,
		_firstPlayerAnnouncementDelay,
		_playerAnnouncementDuration
	);

	auto const owner = GetOwner();
	auto const matchPlayerController = Cast<AMatchPlayerController>(owner);
	AssertIsNotNull(matchPlayerController);

	auto const playerState = matchPlayerController->GetPlayerState<APlayerState>();
	AssertIsNotNull(playerState);

	auto const playerIdValue = playerState->GetPlayerId();
	auto const playerId = PlayerId(playerIdValue);
	
	_announcementManager = announcementManagerFactory.Create(matchGameMode, playerId);
	
	Initialized.Broadcast();
}
