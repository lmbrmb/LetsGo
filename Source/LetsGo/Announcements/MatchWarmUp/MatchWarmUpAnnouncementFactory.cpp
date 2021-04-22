#include "MatchWarmUpAnnouncementFactory.h"
#include "MatchWarmUpAnnouncement.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
IAnnouncement* MatchWarmUpAnnouncementFactory::Create(
)
{
	auto const matchWarmUpAnnouncement = new MatchWarmUpAnnouncement();
	return matchWarmUpAnnouncement;
}
