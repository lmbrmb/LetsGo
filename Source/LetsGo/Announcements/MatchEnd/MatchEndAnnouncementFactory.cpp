#include "MatchEndAnnouncementFactory.h"

#include "MatchEndAnnouncement.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
IAnnouncement* MatchEndAnnouncementFactory::Create(
	const PlayerId& localPlayerId,
	AMatchGameMode* matchGameMode
)
{
	auto const localPlayerPlace = matchGameMode->GetPlayerPlace(localPlayerId);
	auto const matchEndAnnouncement = new MatchEndAnnouncement(localPlayerPlace);
	return matchEndAnnouncement;
}
