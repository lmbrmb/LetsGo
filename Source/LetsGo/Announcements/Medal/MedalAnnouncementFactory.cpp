#include "MedalAnnouncementFactory.h"

#include "MedalAnnouncement.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
IAnnouncement* MedalAnnouncementFactory::Create(
	const Medal& medal,
	const PlayerId& localPlayerId
)
{
	if (medal.GetPlayerId() != localPlayerId)
	{
		return nullptr;
	}

	auto const medalType = medal.GetMedalType();
	auto const medalAnnouncement = new MedalAnnouncement(medalType);
	return medalAnnouncement;
}
