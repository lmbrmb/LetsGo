#include "MatchWarmUpAnnouncementProcessor.h"

#include "LetsGo/Announcements/MatchWarmUp/MatchWarmUpAnnouncement.h"

bool MatchWarmUpAnnouncementProcessor::TryProcessAnnouncement(
	const IAnnouncement* announcement,
	IAnnouncer* announcer
)
{
	auto const matchWarmUpAnnouncement = dynamic_cast<const MatchWarmUpAnnouncement*>(announcement);
	if (!matchWarmUpAnnouncement)
	{
		return false;
	}

	announcer->AnnounceMatchWarmUp();
	
	return true;
}
