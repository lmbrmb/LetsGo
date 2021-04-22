#include "MatchEndAnnouncementProcessor.h"

#include "LetsGo/Announcements/MatchEnd/MatchEndAnnouncement.h"

bool MatchEndAnnouncementProcessor::TryProcessAnnouncement(
	const IAnnouncement* announcement,
	IAnnouncer* announcer
)
{
	auto const matchEndAnnouncement = dynamic_cast<const MatchEndAnnouncement*>(announcement);
	if (!matchEndAnnouncement)
	{
		return false;
	}
	auto const localPlayerPlace = matchEndAnnouncement->GetLocalPlayerPlace();
	announcer->AnnounceMatchEnd(localPlayerPlace);

	return true;
}
