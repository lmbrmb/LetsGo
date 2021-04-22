#include "AnnouncementManagerComponent.h"

#include "AnnouncementManagerFactory.h"
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

	AnnouncementManagerFactory announcementManagerFactory(matchGameMode);
	announcementManagerFactory.SetTimings(
		_matchWarmUpAnnouncementDuration,
		_matchStartAnnouncementDuration,
		_matchEndAnnouncementDuration,
		_firstPlayerAnnouncementDelay,
		_playerAnnouncementDuration
	);
	_announcementManager = announcementManagerFactory.Create();
	
	Initialized.Broadcast();
}
