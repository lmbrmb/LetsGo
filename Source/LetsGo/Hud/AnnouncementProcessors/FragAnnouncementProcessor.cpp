#include "FragAnnouncementProcessor.h"

#include "LetsGo/Announcements/Frag/FragAnnouncement.h"

bool FragAnnouncementProcessor::TryProcessAnnouncement(
	const IAnnouncement* announcement,
	IAnnouncer* announcer
)
{
	auto const fragAnnouncement = dynamic_cast<const FragAnnouncement*>(announcement);
	if (!fragAnnouncement)
	{
		return false;
	}

	announcer->AnnounceFrag(
		fragAnnouncement->GetInstigatorPlayerNickname(),
		fragAnnouncement->GetFraggedPlayerNickname(),
		fragAnnouncement->GetIsLocalPlayerInstigator(),
		fragAnnouncement->GetIsLocalPlayerFragged(),
		fragAnnouncement->GetInstigatorPlayerPlace(),
		fragAnnouncement->GetFraggedPlayerPlace()
	);
	
	return true;
}
