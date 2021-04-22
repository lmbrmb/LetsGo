#include "MatchStartAnnouncementFactory.h"

#include "MatchStartAnnouncement.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
IAnnouncement* MatchStartAnnouncementFactory::Create(
)
{
	auto const matchStartAnnouncement = new MatchStartAnnouncement();
	return matchStartAnnouncement;
}
