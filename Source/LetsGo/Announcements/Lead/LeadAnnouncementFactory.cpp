#include "LeadAnnouncementFactory.h"

#include "LeadAnnouncement.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
IAnnouncement* LeadAnnouncementFactory::Create(
	const FLeadState& leadState
)
{
	auto const leadAnnouncement = new LeadAnnouncement(leadState);
	return leadAnnouncement;
}
