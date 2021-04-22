#include "AnnouncementManagerFactory.h"
#include "AnnouncementManager.h"

AnnouncementManagerFactory::AnnouncementManagerFactory(
	AMatchGameMode* matchGameMode
	) :
	_matchGameMode(matchGameMode)
{
	_fragAnnouncementFactory = new FragAnnouncementFactory();
	_medalAnnouncementFactory = new MedalAnnouncementFactory();
	_matchWarmUpAnnouncementFactory = new MatchWarmUpAnnouncementFactory();
	_matchStartAnnouncementFactory = new MatchStartAnnouncementFactory();
	_matchEndAnnouncementFactory = new MatchEndAnnouncementFactory();
}

void AnnouncementManagerFactory::SetTimings(
	const float matchWarmUpAnnouncementDuration,
	const float matchStartAnnouncementDuration,
	const float matchEndAnnouncementDuration,
	const float firstPlayerAnnouncementDelay,
	const float playerAnnouncementDuration
)
{
	_matchWarmUpAnnouncementDuration = matchWarmUpAnnouncementDuration;
	_matchStartAnnouncementDuration = matchStartAnnouncementDuration;
	_matchEndAnnouncementDuration = matchEndAnnouncementDuration;
	_firstPlayerAnnouncementDelay = firstPlayerAnnouncementDelay;
	_playerAnnouncementDuration = playerAnnouncementDuration;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
IAnnouncementManager* AnnouncementManagerFactory::Create() const
{
	auto const announcementManager = new AnnouncementManager(_matchGameMode);
	announcementManager->SetFactories(
		_fragAnnouncementFactory,
		_medalAnnouncementFactory,
		_matchWarmUpAnnouncementFactory,
		_matchStartAnnouncementFactory,
		_matchEndAnnouncementFactory
	);
	announcementManager->SetTimings(
		_matchWarmUpAnnouncementDuration,
		_matchStartAnnouncementDuration,
		_matchEndAnnouncementDuration,
		_firstPlayerAnnouncementDelay,
		_playerAnnouncementDuration
	);
	return announcementManager;
}
