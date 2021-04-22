#include "MatchStartAnnouncementProcessor.h"

#include "LetsGo/Announcements/MatchStart/MatchStartAnnouncement.h"

bool MatchStartAnnouncementProcessor::TryProcessAnnouncement(
	const IAnnouncement* announcement,
	IAnnouncer* announcer
)
{
	auto const matchStartAnnouncement = dynamic_cast<const MatchStartAnnouncement*>(announcement);
	if (!matchStartAnnouncement)
	{
		return false;
	}

	announcer->AnnounceMatchStart();
	
	return true;
}
