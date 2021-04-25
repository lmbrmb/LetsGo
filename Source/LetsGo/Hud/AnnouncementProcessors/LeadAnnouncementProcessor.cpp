#include "LeadAnnouncementProcessor.h"

#include "LetsGo/Announcements/Lead/LeadAnnouncement.h"

bool LeadAnnouncementProcessor::TryProcessAnnouncement(
	const IAnnouncement* announcement,
	IAnnouncer* announcer
)
{
	auto const leadAnnouncement = dynamic_cast<const LeadAnnouncement*>(announcement);

	if (!leadAnnouncement)
	{
		return false;
	}

	auto const leadState = leadAnnouncement->GetLeadState();
	announcer->AnnounceLead(leadState);
	
	return true;
}
