#include "MedalAnnouncementProcessor.h"

#include "LetsGo/Announcements/Medal/MedalAnnouncement.h"

bool MedalAnnouncementProcessor::TryProcessAnnouncement(
	const IAnnouncement* announcement,
	IAnnouncer* announcer
)
{
	auto const medalAnnouncement = dynamic_cast<const MedalAnnouncement*>(announcement);

	if (!medalAnnouncement)
	{
		return false;
	}

	auto const medalType = medalAnnouncement->GetMedalType();
	announcer->AnnounceMedal(medalType);
	
	return true;
}
