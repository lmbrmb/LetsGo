#pragma once

#include "LetsGo/Announcements/IAnnouncement.h"
#include "LetsGo/Announcements/IAnnouncementFactory.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/GameModes/MatchGameMode.h"

///<summary>
/// Medal announcement factory
///</summary>
class MedalAnnouncementFactory final : public IAnnouncementFactory
{
public:
	IAnnouncement* Create(
		const Medal& medal,
		const PlayerId& localPlayerId
	);
};
